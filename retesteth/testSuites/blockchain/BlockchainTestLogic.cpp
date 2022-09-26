#include "BlockchainTestLogic.h"
#include <retesteth/EthChecks.h>
#include <retesteth/ExitHandler.h>
#include <retesteth/Options.h>
#include <retesteth/session/Session.h>
#include <retesteth/testStructures/Common.h>
#include <retesteth/testStructures/PrepareChainParams.h>
#include <retesteth/testSuites/Common.h>
#include "fillers/TestBlockchain.h"
using namespace std;
using namespace test::debug;
using namespace test::session;
namespace test
{

/// Read and execute the test from the file
void RunTest(BlockchainTestInFilled const& _test, TestSuite::TestSuiteOptions const& _opt)
{
    ETH_DC_MESSAGE(DC::TESTLOG, "Running " + _test.testName());

    ClientConfig const& cfg = Options::get().getDynamicOptions().getCurrentConfig();
    if (!cfg.validateForkAllowed(_test.network(), false))
        return;

    TestOutputHelper::get().setCurrentTestName(_test.testName());
    TestOutputHelper::get().setUnitTestExceptions(_test.unitTestExceptions());
    SessionInterface& session = RPCSession::instance(TestOutputHelper::getThreadID());

    TestOutputHelper::get().setCurrentTestInfo(TestInfo(_test.network().asString(), 0));
    session.test_setChainParams(prepareChainParams(_test.network(), _test.sealEngine(), _test.Pre(), _test.Env()));

    // for all blocks
    size_t blockNumber = 0;
    for (BlockchainTestBlock const& tblock : _test.blocks())
    {
        if (ExitHandler::receivedExitSignal())
            return;
        if (Options::get().blockLimit != 0 && blockNumber + 1 >= Options::get().blockLimit)
            break;

        TestOutputHelper::get().setCurrentTestInfo(TestInfo(_test.network().asString(), blockNumber++));

        // Transaction sequence validation
        ETH_DC_MESSAGEC(DC::LOWLOG, "PERFORM TRANSACTION SEQUENCE VALIDATION...", LogColor::YELLOW);

        bool atLeastOnceSequence = false;
        typedef std::tuple<spTransaction, string> SpTrException;
        std::map<FH32, SpTrException> expectedExceptions;
        if (tblock.transactionSequence().size())
        {
            if (tblock.header()->number() > session.eth_blockNumber())
            {
                atLeastOnceSequence = true;
                session.test_modifyTimestamp(tblock.header()->timestamp());
            }
            else
                ETH_WARNING("Skipping invalid transaction exception check in reorg block! (calling rewind not good when running the test, use filltests instead)");
        }
        for (auto const& el : tblock.transactionSequence())
        {
            spTransaction const& tr = std::get<0>(el);
            string const& sException = std::get<1>(el);
            session.eth_sendRawTransaction(tr->getRawBytes(), tr->getSecret());
            SpTrException info = {tr, sException};
            expectedExceptions.emplace(tr->hash(), info);
        }
        if (atLeastOnceSequence)
        {
            VALUE const& origNumber = session.eth_blockNumber();
            MineBlocksResult const miningRes = session.test_mineBlocks(1);
            for (auto const& el : expectedExceptions)
            {
                SpTrException const& res = el.second;
                spTransaction resTransaction = std::get<0>(res);
                string const& testException = std::get<1>(res);
                compareTransactionException(resTransaction, miningRes, testException);
            }
            session.test_rewindToBlock(origNumber);
        }
        // transaction sequence validation

        FH32 const blHash(session.test_importRawBlock(tblock.rlp()));

        if (!session.getLastRPCError().empty())
        {
            // and options does NOT allow invalid blocks or this block was expected to be valid
            // BCValidBlocks does not allow invalid blocks!
            if (!_opt.allowInvalidBlocks || !tblock.expectedInvalid())
                ETH_ERROR_MESSAGE(
                    "Importing raw RLP block, block was expected to be valid! (if it was intended, check that it is not in Valid blocks test suite) " + session.getLastRPCError().message());

            if (Options::getDynamicOptions().getCurrentConfig().cfgFile().socketType() == ClientConfgSocketType::TransitionTool)
            {
                string const& sBlockException = tblock.getExpectException();
                if (!sBlockException.empty())
                    blockchainfiller::TestBlockchain::checkBlockException(session, sBlockException);
            }
            continue;
        }
        else
            ETH_ERROR_REQUIRE_MESSAGE(!tblock.expectedInvalid(),
                "Running blockchain test: Block #" + to_string(blockNumber) + " expected to be invalid!");

        // Check imported block against the fields in test
        // Check Blockheader
        EthGetBlockBy latestBlock(session.eth_getBlockByHash(blHash, Request::FULLOBJECTS));

        for (auto const& tr : tblock.transactions())
        {
            if (Options::get().vmtrace)
            {
                string const testNameOut = _test.testName() + "_" + tr->hash().asString() + ".txt";
                VMtraceinfo info(session, tr->hash(), latestBlock.header()->stateRoot(), testNameOut);
                printVmTrace(info);
            }
        }

        spDataObject remoteHeader = latestBlock.header()->asDataObject();
        spDataObject testHeader = tblock.header()->asDataObject();
        bool condition = remoteHeader->asJson(0, false) == testHeader->asJson(0, false);
        /*if (_opt.isLegacyTests)
        {
            inTestHeader = bdata.atKey("blockHeader");  // copy!!!
            if (latestBlock.getBlockHeader().atKey("nonce").asString() == "0x0000000000000000")
            {
                // because on NoProof mixHash is returned as 0x00 by other client, even if block
                // imported with mixHash/nonce ??? who returned 0x00 as mixHash??? wtf
                inTestHeader["mixHash"] =
                    "0x0000000000000000000000000000000000000000000000000000000000000000";
                inTestHeader["nonce"] = "0x0000000000000000";
            }
            inTestHeader["hash"] = latestBlock.getBlockHeader().atKey("hash");
            condition = latestBlock.getBlockHeader() == inTestHeader;
        }*/

        // string const jsonHeader = inTestHeader.type() == DataType::Null ?
        //                              bdata.atKey("blockHeader").asJson() :
        //                              "(adjusted)" + inTestHeader.asJson();
        string message;
        if (!condition)
        {
            string errField;
            message = "Client return HEADER vs Test HEADER: \n";
            message += compareBlockHeaders(remoteHeader.getCContent(), testHeader.getCContent(), errField);
        }
        ETH_ERROR_REQUIRE_MESSAGE(
            condition, "Client report different blockheader after importing the rlp than expected by test! \n" + message);

        // Verify uncles to one described in the fields
        size_t ind = 0;
        message = "Client return UNCLES: " + to_string(latestBlock.uncles().size()) + " vs " +
                  "Test UNCLES: " + to_string(tblock.uncles().size());
        ETH_ERROR_REQUIRE_MESSAGE(latestBlock.uncles().size() == tblock.uncles().size(),
            "Client report different uncle count after importing the rlp than expected by test! \n" + message);
        for (spBlockHeader const& tuncle : tblock.uncles())
        {
            FH32 clientUncleHash = latestBlock.uncles().at(ind++);  // EthGetBlockBy return only hashes
            if (clientUncleHash != tuncle->hash())
                ETH_ERROR_MESSAGE("Remote client returned block with unclehash that is not expected by test! " + message);
        }

        // Check Transaction count
        message = "Client return TRANSACTIONS: " + to_string(latestBlock.transactions().size()) + " vs " +
                  "Test TRANSACTIONS: " + to_string(tblock.transactions().size());
        ETH_ERROR_REQUIRE_MESSAGE(latestBlock.transactions().size() == tblock.transactions().size(),
            "Client report different transaction count after importing the rlp than expected by test! \n" + message);

        // Verify transactions to one described in the fields
        ind = 0;
        for (spTransaction const& tr : tblock.transactions())
        {
            if (ExitHandler::receivedExitSignal())
                return;
            EthGetBlockByTransaction const& clientTr = latestBlock.transactions().at(ind++);
            ETH_ERROR_REQUIRE_MESSAGE(clientTr.blockHash() == tblock.header()->hash(),
                "Error checking remote transaction, remote tr `blockHash` is different to one described in test block! "
                "(" +
                    clientTr.blockHash().asString() + " != " + tblock.header()->hash().asString() + ")");

            ETH_ERROR_REQUIRE_MESSAGE(clientTr.blockNumber() == tblock.header()->number(),
                "Error checking remote transaction, remote tr `blockNumber` is different to one described in test block! "
                "(" +
                    clientTr.blockNumber().asDecString() + " != " + tblock.header()->number().asDecString() + ")");

            BYTES const testTr = tr->getRawBytes();
            BYTES const remoteTr = clientTr.transaction()->getRawBytes();

            ETH_ERROR_REQUIRE_MESSAGE(remoteTr == testTr, "Error checking remote transaction, remote tr `" +
                                                              remoteTr.asString() + "` is different to test tr `" +
                                                              testTr.asString() + "`)");
        }
    }

    EthGetBlockBy latestBlock(session.eth_getBlockByNumber(session.eth_blockNumber(), Request::LESSOBJECTS));
    if (_test.isFullState())
        compareStates(_test.Post(), session);
    else
        ETH_ERROR_REQUIRE_MESSAGE(_test.PostHash() == latestBlock.header()->stateRoot(),
            "postStateHash mismatch! remote: " + latestBlock.header()->stateRoot().asString() + " != test " =
                _test.PostHash().asString());

    if (_test.lastBlockHash() != latestBlock.header()->hash())
        ETH_ERROR_MESSAGE("lastblockhash does not match! remote: '" + latestBlock.header()->hash().asString() +
                          "', test: '" + _test.lastBlockHash().asString() + "'");

    bool skipGenesisCheck = false;
    if (_opt.isLegacyTests)
    {
        // Skip old generated tests (Legacy) genesis check on aleth
        // because mixHash and nonce return is different to geth
        if (Options::getDynamicOptions().getCurrentConfig().cfgFile().path().parent_path().filename() == "aleth")
            skipGenesisCheck = true;

        if (_test.genesisRLP().asString() == "0x00")
            skipGenesisCheck = true;
    }

    if (!skipGenesisCheck)
    {
        EthGetBlockBy genesis(session.eth_getBlockByNumber(0, Request::LESSOBJECTS));
        if (_test.genesisRLP() != genesis.getRLPHeaderTransactions())
            ETH_ERROR_MESSAGE("genesisRLP in test != genesisRLP on remote client! (" + _test.genesisRLP().asString() +
                              "' != '" + genesis.getRLPHeaderTransactions().asString() + "'");
    }
}

spDataObject DoTests(spDataObject& _input, TestSuite::TestSuiteOptions& _opt)
{
    spDataObject tests;
    if (_opt.doFilling)
    {
        BlockchainTestFiller testFiller(_input);

        // Test Name Warning
        string const tFileName = TestOutputHelper::get().testFile().stem().string();
        string const tObjectName = testFiller.tests().at(0).testName() + "Filler";
        if (tObjectName != tFileName)
            ETH_WARNING("Blockchain test filler first test name is not equal test filename! (" + tObjectName +
                        " != " + tFileName + ")");

        for (BlockchainTestInFiller const& bcTest : testFiller.tests())
        {
            // Select test by name if --singletest and --singlenet is set
            if (Options::get().singletest.initialized())
            {
                if (!Options::get().singletest.subname.empty() && bcTest.testName() != Options::get().singletest.subname)
                    continue;
            }
            if (!Options::get().singleTestNet.empty())
            {
                if (!bcTest.hasExpectForNetwork(FORK(Options::get().singleTestNet)))
                    continue;
            }

            // One blockchain test generate many tests for each network
            spDataObject filledTests = FillTest(bcTest, _opt);
            for (auto const& t : (*filledTests).getSubObjects())
                (*tests).addSubObject(t);
            TestOutputHelper::get().registerTestRunSuccess();
        }
    }
    else
    {
        TestOutputHelper::get().setCurrentTestInfo(TestInfo("Parsing blockchain test"));
        TestOutputHelper::get().setCurrentTestName("----");
        if (_opt.isLegacyTests)
        {
            for (auto& test : _input.getContent().getSubObjectsUnsafe())
            {
                if (!test.getContent().count("genesisRLP"))
                    test.getContent()["genesisRLP"] = "0x00";
            }
        }

        ETH_DC_MESSAGE(DC::TESTLOG, "Parse test");
        BlockchainTest test(_input);
        // Just check the test structure if running with --checkhash
        if (Options::get().checkhash)
            return tests;
        ETH_DC_MESSAGE(DC::TESTLOG, "Parse test done");

        for (BlockchainTestInFilled const& bcTest : test.tests())
        {
            // Select test by name if --singletest and --singlenet is set
            if (Options::get().singletest.initialized())
            {
                if (!Options::get().singletest.subname.empty() && bcTest.testName() != Options::get().singletest.subname)
                    continue;
            }

            if (!Options::get().singleTestNet.empty())
            {
                if (bcTest.network().asString() != Options::get().singleTestNet)
                    continue;
            }
            RunTest(bcTest, _opt);
            TestOutputHelper::get().registerTestRunSuccess();
        }
    }
    return tests;
}
}  // namespace test
