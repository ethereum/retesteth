#include "BlockchainTestLogic.h"
#include <retesteth/EthChecks.h>
#include <retesteth/RPCSession.h>
#include <retesteth/testSuites/Common.h>

void premineBlockHeader(
    RPCSession& _session, scheme_blockchainTestFiller::blockSection const& _block);
test::scheme_block postmineBlockHeader(RPCSession& _session,
    scheme_blockchainTestFiller::blockSection const& _block, BlockNumber const& _latestBlockNumber,
    string const& _network);

namespace test
{
/// Generate blockchain test from filler
void FillTest(scheme_blockchainTestFiller const& _testObject, string const& _network,
    TestSuite::TestSuiteOptions const&, DataObject& _testOut)
{
    // construct filled blockchain test
    _testOut["sealEngine"] = _testObject.getSealEngine();
    _testOut["network"] = _network;
    _testOut["pre"] = _testObject.getPre().getData();
    if (_testObject.getData().count("_info"))
        _testOut["_info"] = _testObject.getData().atKey("_info");

    RPCSession& session = RPCSession::instance(TestOutputHelper::getThreadID());
    DataObject genesisObject = _testObject.getGenesisForRPC(_network);
    session.test_setChainParams(genesisObject.asJson());

    test::scheme_block latestBlock = session.eth_getBlockByNumber(BlockNumber("0"), false);
    _testOut["genesisBlockHeader"] = latestBlock.getBlockHeader();
    _testOut["genesisBlockHeader"].removeKey("transactions");
    _testOut["genesisBlockHeader"].removeKey("uncles");
    // u256 genesisTimestamp =
    // std::time(0);//u256(genesisObject.atKey("genesis").atKey("timestamp").asString());

    size_t number = 0;
    for (auto const& block : _testObject.getBlocks())
    {
        session.test_modifyTimestamp(1000);  // Shift block timestamp relative to previous block
        TestInfo errorInfo (_network, number++);
        TestOutputHelper::get().setCurrentTestInfo(errorInfo);

        // premineBlockHeader(session, block);
        DataObject blockSection;
        for (auto const& tr : block.getTransactions())
        {
            session.eth_sendRawTransaction(tr.getSignedRLP());
            blockSection["transactions"].addArrayObject(tr.getDataForBCTest());
        }

        string latestBlockNumber = session.test_mineBlocks(1);
        if (block.getData().count("blockHeader"))
        {
            latestBlock = postmineBlockHeader(session, block, latestBlockNumber, _network);
            if (!latestBlock.isValid())
                blockSection.removeKey("transactions");
        }
        else
        {
            // SOME TRANSACTIONS MIGHT BE EXPECTED TO FAIL
            latestBlock = session.eth_getBlockByNumber(latestBlockNumber, true);
        }

        ETH_ERROR_REQUIRE_MESSAGE(
            latestBlock.getTransactionCount() == block.getTransactions().size(),
            "BlockchainTest transaction execution failed! (remote " +
                toString(latestBlock.getTransactionCount()) + " != test " +
                toString(block.getTransactions().size()) + ")");

        blockSection["rlp"] = latestBlock.getBlockRLP();
        if (latestBlock.isValid())
            blockSection["blockHeader"] = latestBlock.getBlockHeader();
        else
            blockSection["expectException"] = block.getException(_network);

        _testOut["blocks"].addArrayObject(blockSection);
        if (!latestBlock.isValid())
        {
            latestBlockNumber = session.eth_blockNumber();
            latestBlock = session.eth_getBlockByNumber(latestBlockNumber, false);
        }
    }

    scheme_state remoteState = getRemoteState(session, latestBlock);
    if (remoteState.isHash())
        compareStates(_testObject.getExpectSection().getExpectSectionFor(_network).getExpectState(),
            session, latestBlock);
    else
        compareStates(_testObject.getExpectSection().getExpectSectionFor(_network).getExpectState(),
            remoteState);

    if (remoteState.isHash())
        _testOut["postStateHash"] = remoteState.getData();
    else
        _testOut["postState"] = remoteState.getData();
    _testOut["lastblockhash"] = latestBlock.getBlockHash();
}

/// Read and execute the test from the file
void RunTest(DataObject const& _testObject, TestSuite::TestSuiteOptions const& _opt)
{
    if (Options::get().logVerbosity > 1)
        std::cout << "Running " << TestOutputHelper::get().testName() << std::endl;
    scheme_blockchainTest inputTest(_testObject);
    RPCSession& session = RPCSession::instance(TestOutputHelper::getThreadID());

    // Info for genesis
    TestInfo errorInfo (inputTest.getNetwork(), 0);
    TestOutputHelper::get().setCurrentTestInfo(errorInfo);
    session.test_setChainParams(inputTest.getGenesisForRPC(inputTest.getNetwork()).asJson());

    // for all blocks
    size_t blockNumber = 0;
    for (auto const& brlp : inputTest.getBlockRlps())
    {
        TestInfo errorInfo(inputTest.getNetwork(), blockNumber++);
        TestOutputHelper::get().setCurrentTestInfo(errorInfo);
        session.test_importRawBlock(brlp);
        if (session.getLastRPCError().type() != DataType::Null)
        {
            if (!_opt.allowInvalidBlocks)
                ETH_ERROR_MESSAGE("Running blockchain test: " + session.getLastRPCError().atKey("message").asString());
            else
                std::cerr << session.getLastRPCError().atKey("error").asString() << std::endl;
        }
    }

    // wait for blocks to process
    // std::this_thread::sleep_for(std::chrono::seconds(10));

    scheme_block latestBlock = session.eth_getBlockByNumber(session.eth_blockNumber(), false);
    if (inputTest.getPost().isHash())
        validatePostHash(session, inputTest.getPost().getHash(), latestBlock);
    else
        compareStates(scheme_expectState(inputTest.getPost().getData()), session, latestBlock);
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
            if (Options::get().singleTest)
            {
                if (!Options::get().singleSubTestName.empty() &&
                    testname != Options::get().singleSubTestName)
                    continue;
            }

            if (!Options::get().singleTestNet.empty())
            {
                if (i.count("network") &&
                    i.atKey("network").asString() != Options::get().singleTestNet)
                    continue;
            }

            RunTest(i, _opt);
        }
    }
    TestOutputHelper::get().registerTestRunSuccess();
    return tests;
}
}  // namespace test

void premineBlockHeader(RPCSession&, scheme_blockchainTestFiller::blockSection const&) {}

test::scheme_block postmineBlockHeader(RPCSession& _session,
    scheme_blockchainTestFiller::blockSection const& _block, BlockNumber const& _latestBlockNumber,
    string const& _network)
{
    // if blockHeader is defined in test Filler, rewrite the last block header fields with info from
    // test and reimport it to the client in order to trigger an exception in the client
    test::scheme_block remoteBlock = _session.eth_getBlockByNumber(_latestBlockNumber, true);
    DataObject header = remoteBlock.getBlockHeader();

    for (auto const& replace : _block.getData().atKey("blockHeader").getSubObjects())
    {
        if (replace.getKey() == "updatePoW" || replace.getKey() == "expectException")
            continue;
        if (replace.getKey() == "RelTimestamp")
        {
            BlockNumber previousBlockNumber(_latestBlockNumber);
            previousBlockNumber.applyShift(-1);

            test::scheme_block previousBlock =
                _session.eth_getBlockByNumber(previousBlockNumber, false);
            string previousBlockTimestampString =
                previousBlock.getBlockHeader().atKey("timestamp").asString();
            BlockNumber previousBlockTimestamp(previousBlockTimestampString);
            previousBlockTimestamp.applyShift(replace.asString());
            header["timestamp"] = previousBlockTimestamp.getBlockNumberAsString();
            continue;
        }
        if (header.count(replace.getKey()))
            header[replace.getKey()] = replace.asString();
        else
            ETH_STDERROR_MESSAGE(
                "blockHeader field in test filler tries to overwrite field that is not found in "
                "blockheader: '" +
                replace.getKey() + "'");
    }

    // replace block with overwritten header
    remoteBlock.overwriteBlockHeader(header);
    _session.test_rewindToBlock(_latestBlockNumber.getBlockNumberAsInt() - 1);
    _session.test_importRawBlock(remoteBlock.getBlockRLP());

    // check malicious block import exception
    if (_block.getException(_network) == "NoException")
        ETH_ERROR_REQUIRE_MESSAGE(_session.getLastRPCErrorMessage().empty(),
            "Postmine block tweak expected no exception! Client errors with: '" +
                _session.getLastRPCErrorMessage() + "'");
    else
    {
        std::string const& clientExceptionString =
            Options::get().getDynamicOptions().getCurrentConfig().getExceptionString(
                _block.getException(_network));
        size_t pos = _session.getLastRPCErrorMessage().find(clientExceptionString);
        if (clientExceptionString.empty())
            pos = string::npos;
        ETH_ERROR_REQUIRE_MESSAGE(pos != string::npos,
            "'" + clientExceptionString + "' (" + _block.getException(_network) +
                ") not found in client response to postmine block tweak! Import result of postmine "
                "block: '" +
                _session.getLastRPCErrorMessage() + "', Test Expected: '" + clientExceptionString +
                "'");
        remoteBlock.setValid(false);
    }
    return remoteBlock;  // malicious block must be written to the filled test
}
