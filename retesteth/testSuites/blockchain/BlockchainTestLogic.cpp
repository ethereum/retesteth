#include "BlockchainTestLogic.h"
#include <retesteth/EthChecks.h>
#include <retesteth/RPCSession.h>
#include <retesteth/testSuites/Common.h>

namespace test
{
/// Generate blockchain test from filler
bool FillTest(scheme_blockchainTestFiller const& _testObject, string const& _network,
    TestSuite::TestSuiteOptions const&, DataObject& _testOut)
{
    // construct filled blockchain test
    _testOut["sealEngine"] = _testObject.getSealEngine();
    _testOut["network"] = _network;
    _testOut["pre"] = _testObject.getData().atKey("pre");
    if (_testObject.getData().count("_info"))
        _testOut["_info"] = _testObject.getData().atKey("_info");

    RPCSession& session = RPCSession::instance(TestOutputHelper::getThreadID());
    DataObject genesisObject = _testObject.getGenesisForRPC(_network);
    session.test_setChainParams(genesisObject.asJson());

    _testOut["genesisBlockHeader"] = session.eth_getBlockByNumber("0", false).getBlockHeader();
    _testOut["genesisBlockHeader"].removeKey("transactions");
    _testOut["genesisBlockHeader"].removeKey("uncles");
    // u256 genesisTimestamp = u256(genesisObject.atKey("genesis").atKey("timestamp").asString());
    // session.test_modifyTimestamp(genesisTimestamp.convert_to<size_t>());

    string lastTrHash;
    string lastBlHash;
    size_t number = 0;
    for (auto const& block : _testObject.getBlocks())
    {
        TestOutputHelper::get().setCurrentTestInfo("Network: " + _network +
                                                   ", BlockNumber: " + toString(number++) +
                                                   ", Test: " + TestOutputHelper::get().testName());

        DataObject blockSection;
        for (auto const& tr : block.getTransactions())
        {
            lastTrHash = session.eth_sendRawTransaction(tr.getSignedRLP());
            if (!session.getLastRPCError().empty())
                ETH_ERROR_MESSAGE(session.getLastRPCError());
            if (!isHash<h256>(lastTrHash))
                ETH_ERROR_MESSAGE("eth_sendRawTransaction return invalid hash: '" + lastTrHash +
                                  "' " + TestOutputHelper::get().testInfo());
            blockSection["transactions"].addArrayObject(tr.getData());
        }
        session.test_mineBlocks(1);

        DataObject remoteState = getRemoteState(session, "", false);
        test::scheme_block blockData(remoteState.atKey("rawBlockData"));
        ETH_ERROR_REQUIRE_MESSAGE(blockData.getTransactionCount() == block.getTransactions().size(),
            "BlockchainTest transaction execution failed! " + TestOutputHelper::get().testInfo());
        blockSection["rlp"] = blockData.getBlockRLP();
        blockSection["blockHeader"] = blockData.getBlockHeader();
        _testOut["blocks"].addArrayObject(blockSection);
    }

    DataObject remoteState = getRemoteState(session, lastTrHash, true);
    scheme_state postState(remoteState.atKey("postState"));
    CompareResult res = test::compareStates(
        _testObject.getExpectSection().getExpectSectionFor(_network).getExpectState(), postState);
    ETH_ERROR_REQUIRE_MESSAGE(res == CompareResult::Success, TestOutputHelper::get().testInfo());
    if (res != CompareResult::Success)
        return true;
    _testOut["postState"] = remoteState.atKey("postState");
    _testOut["lastblockhash"] = lastBlHash;
    return false;
}

/// Read and execute the test from the file
bool RunTest(DataObject const& _testObject, TestSuite::TestSuiteOptions const& _opt)
{
    scheme_blockchainTest inputTest(_testObject);
    RPCSession& session = RPCSession::instance(TestOutputHelper::getThreadID());

    session.test_setChainParams(inputTest.getGenesisForRPC(inputTest.getNetwork()).asJson());

    // for all blocks
    for (auto const& brlp : inputTest.getBlockRlps())
    {
        session.test_importRawBlock(brlp);
        if (!session.getLastRPCError().empty() && !_opt.allowInvalidBlocks)
            ETH_ERROR_MESSAGE("Running blockchain test: " + session.getLastRPCError());
    }

    // wait for blocks to process
    // std::this_thread::sleep_for(std::chrono::seconds(10));

    // compare post state hash
    DataObject remoteState;
    if (inputTest.getPost().getHash().empty())
    {
        remoteState = getRemoteState(session, "", true);
        scheme_state postState(remoteState.atKey("postState"));
        CompareResult res = test::compareStates(inputTest.getPost(), postState);
        ETH_ERROR_REQUIRE_MESSAGE(
            res == CompareResult::Success, "Error in " + inputTest.getData().getKey());
        return (res != CompareResult::Success);
    }
    else
    {
        remoteState = getRemoteState(session, "", false);
        bool flag = remoteState.atKey("postHash").asString() == inputTest.getPost().getHash();
        ETH_ERROR_REQUIRE_MESSAGE(flag, "Error in " + TestOutputHelper::get().testInfo() +
                                            ", Expected post state `" +
                                            inputTest.getPost().getHash() + "', but got: '" +
                                            remoteState.atKey("postHash").asString() + "'");
        return flag;
    }
}

DataObject DoTests(DataObject const& _input, TestSuite::TestSuiteOptions& _opt)
{
    checkDataObject(_input);
    checkAtLeastOneTest(_input);

    DataObject tests;
    // A blockchain test file contains many tests in one .json file
    for (auto const& i : _input.getSubObjects())
    {
        string const& testname = i.getKey();
        TestOutputHelper::get().setCurrentTestName(testname);

        if (_opt.doFilling)
        {
            scheme_blockchainTestFiller testFiller(i);
            // Create a blockchain test for each network described in expect section
            for (auto& network : testFiller.getExpectSection().getAllNetworksFromExpectSection())
            {
                // select expect section corresponding to the network (if any)
                for (auto const& expect : testFiller.getExpectSection().getExpectSections())
                {
                    if (expect.getNetworks().count(network))
                    {
                        string const newtestname = testname + "_" + network;
                        TestOutputHelper::get().setCurrentTestName(newtestname);

                        DataObject testOutput;
                        _opt.wasErrors = FillTest(testFiller, network, _opt, testOutput);
                        if (testFiller.getData().count("_info"))
                            testOutput["_info"] = testFiller.getData().atKey("_info");
                        tests[newtestname] = testOutput;
                    }
                }
            }
        }
        else
        {
            // Select test by name if --singletest and --singlenet is set
            if (!Options::get().singleTestNet.empty() && Options::get().singleTest)
                if (testname != Options::get().singleTestName + "_" + Options::get().singleTestNet)
                    continue;

            _opt.wasErrors = RunTest(i, _opt);
        }
    }

    return tests;
}


}  // namespace closed
