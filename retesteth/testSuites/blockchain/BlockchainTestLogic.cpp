#include "BlockchainTestLogic.h"
#include <retesteth/EthChecks.h>
#include <retesteth/RPCSession.h>
#include <retesteth/testSuites/Common.h>

namespace test
{
/// Generate blockchain test from filler
void FillTest(scheme_blockchainTestFiller const& _testObject, string const& _network,
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
    // u256 genesisTimestamp =
    // std::time(0);//u256(genesisObject.atKey("genesis").atKey("timestamp").asString());

    string lastTrHash;
    string lastBlHash;
    size_t number = 0;
    for (auto const& block : _testObject.getBlocks())
    {
        session.test_modifyTimestamp(1000);  // Shift block timestamp relative to previous block
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

        scheme_remoteState remoteState = getRemoteState(session, StateRequest::AttemptFullPost);
        test::scheme_block blockData(remoteState.getData().atKey("rawBlockData"));

        // SOME TRANSACTIONS MIGHT BE EXPECTED TO FAIL
        ETH_ERROR_REQUIRE_MESSAGE(blockData.getTransactionCount() == block.getTransactions().size(),
            "BlockchainTest transaction execution failed! " + TestOutputHelper::get().testInfo());
        blockSection["rlp"] = blockData.getBlockRLP();
        blockSection["blockHeader"] = blockData.getBlockHeader();
        _testOut["blocks"].addArrayObject(blockSection);
        lastBlHash = blockData.getBlockHash();
    }

    ExpectInfo info;
    scheme_remoteState remoteState;
    info.expectState =
        _testObject.getExpectSection().getExpectSectionFor(_network).getExpectState();
    checkExpectSection(session, info, remoteState);
    _testOut["postState"] = remoteState.getPostState();
    _testOut["lastblockhash"] = lastBlHash;
}

/// Read and execute the test from the file
void RunTest(DataObject const& _testObject, TestSuite::TestSuiteOptions const& _opt)
{
    scheme_blockchainTest inputTest(_testObject);
    RPCSession& session = RPCSession::instance(TestOutputHelper::getThreadID());
    string testInfo = TestOutputHelper::get().testName() + ", fork: " + inputTest.getNetwork();
    TestOutputHelper::get().setCurrentTestInfo(testInfo);

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

    ExpectInfo info;
    if (!inputTest.getPost().isHash().empty())  // Blockchain test has state as a hash
        info.postHash = inputTest.getPost().isHash();
    else
        info.expectState = scheme_expectState(inputTest.getPost().getData());
    scheme_remoteState remoteState;
    checkExpectSection(session, info, remoteState);
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
                        FillTest(testFiller, network, _opt, testOutput);
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

            RunTest(i, _opt);
        }
    }

    return tests;
}


}  // namespace closed
