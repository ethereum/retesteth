#include "BlockchainTestLogic.h"
#include "fillers/BlockchainTestFillerLogic.h"
#include <retesteth/EthChecks.h>
#include <retesteth/session/RPCSession.h>
#include <retesteth/testStructures/Common.h>
#include <retesteth/testSuites/Common.h>

namespace test
{

/// Read and execute the test from the file
void RunTest(BlockchainTestInFilled const& _test, TestSuite::TestSuiteOptions const& _opt)
{
    (void)_opt;
    if (Options::get().logVerbosity > 1)
        ETH_STDOUT_MESSAGE("Running" + _test.testName());

    TestOutputHelper::get().setUnitTestExceptions(_test.unitTestExceptions());
    SessionInterface& session = RPCSession::instance(TestOutputHelper::getThreadID());

    TestOutputHelper::get().setCurrentTestInfo(TestInfo(_test.network().asString(), 0));
    DataObject request = prepareChainParams(_test.network(), _test.sealEngine(), _test.Pre(), _test.Env());
    session.test_setChainParams(request);

    // for all blocks
    size_t blockNumber = 0;
    for (BlockchainTestBlock const& tblock : _test.blocks())
    {
        if (Options::get().blockLimit != 0 && blockNumber + 1 >= Options::get().blockLimit)
            break;

        TestOutputHelper::get().setCurrentTestInfo(TestInfo(_test.network().asString(), blockNumber++));
        FH32 const blHash(session.test_importRawBlock(tblock.rlp()));

        if (!session.getLastRPCError().empty())
        {
            // If there was an error on block rlp import
            // and options does NOT allow invalid blocks or this block was expected to be valid
            if (!_opt.allowInvalidBlocks || !tblock.expectedInvalid())
                ETH_ERROR_MESSAGE("Running blockchain test: " + session.getLastRPCError().message());
        }

        // Check imported block against the fields in test
        // Check Blockheader
        EthGetBlockBy latestBlock(session.eth_getBlockByHash(blHash, Request::FULLOBJECTS));
        bool condition = latestBlock.header() == tblock.header();
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
        string message = "Client return HEADER: " + latestBlock.header().asDataObject().asJson() + "\n vs \n" +
                         "Test HEADER: " + tblock.header().asDataObject().asJson();
        ETH_ERROR_REQUIRE_MESSAGE(
            condition, "Client report different blockheader after importing the rlp than expected by test! \n" + message);

        // Verify uncles to one described in the fields
        size_t ind = 0;
        message = "Client return UNCLES: " + to_string(latestBlock.uncles().size()) + " vs " +
                  "Test UNCLES: " + to_string(tblock.uncles().size());
        ETH_ERROR_REQUIRE_MESSAGE(latestBlock.uncles().size() == tblock.uncles().size(),
            "Client report different uncle count after importing the rlp than expected by test! \n" + message);
        for (BlockHeader const& tuncle : tblock.uncles())
        {
            FH32 clientUncleHash = latestBlock.uncles().at(ind++);  // EthGetBlockBy return only hashes
            message = "(" + clientUncleHash.asString() + " != " + tuncle.hash().asString() + ")";
            if (clientUncleHash != tuncle.hash())
                ETH_ERROR_MESSAGE("Remote client returned block with unclehash that is not expected by test! " + message);
        }

        // Check Transaction count
        message = "Client return TRANSACTIONS: " + to_string(latestBlock.transactions().size()) + " vs " +
                  "Test TRANSACTIONS: " + to_string(tblock.transactions().size());
        ETH_ERROR_REQUIRE_MESSAGE(latestBlock.transactions().size() == tblock.transactions().size(),
            "Client report different transaction count after importing the rlp than expected by test! \n" + message);

        // Verify transactions to one described in the fields
        ind = 0;
        for (Transaction const& tr : tblock.transactions())
        {
            EthGetBlockByTransaction const& clientTr = latestBlock.transactions().at(ind++);
            ETH_ERROR_REQUIRE_MESSAGE(clientTr.blockHash() == tblock.header().hash(),
                "Error checking remote transaction, remote tr `blockHash` is different to one described in test block!"
                "(" +
                    clientTr.blockHash().asString() + " != " + tblock.header().hash().asString() + ")");

            ETH_ERROR_REQUIRE_MESSAGE(clientTr.blockNumber() == tblock.header().number(),
                "Error checking remote transaction, remote tr `blockNumber` is different to one described in test block!"
                "(" +
                    clientTr.blockNumber().asDecString() + " != " + tblock.header().number().asDecString() + ")");

            DataObject const testTr = tr.asDataObject();
            DataObject const remoteTr = clientTr.transaction().asDataObject();
            ETH_ERROR_REQUIRE_MESSAGE(remoteTr == testTr, "Error checking remote transaction, remote tr `" + remoteTr.asJson() +
                                                              "` is different to test tr `" + testTr.asJson() + "`)");
        }
    }

    EthGetBlockBy latestBlock(session.eth_getBlockByNumber(session.eth_blockNumber(), Request::LESSOBJECTS));
    compareStates(_test.Post(), session);

    if (_test.lastBlockHash() != latestBlock.header().hash())
        ETH_ERROR_MESSAGE("lastblockhash does not match! remote: " + latestBlock.header().hash().asString() +
                          ", test: " + _test.lastBlockHash().asString());

    // if (_test.genesisRLP())
    //{
    //    if (_opt.isLegacyTests &&
    //        Options::getDynamicOptions().getCurrentConfig().getFolderName() == "aleth")
    //    {
    // skip old generated tests (Legacy) genesis check on aleth
    // because mixHash and nonce return is different to geth
    //    }
    //    else
    {
        EthGetBlockBy genesis(session.eth_getBlockByNumber(0, Request::LESSOBJECTS));
        if (_test.genesisRLP() != genesis.fakeRLP())
            ETH_ERROR_MESSAGE("genesisRLP in test != genesisRLP on remote client! (" + _test.genesisRLP().asString() +
                              "' != '" + genesis.fakeRLP().asString() + "'");
        }
        //}
}

DataObject DoTests(DataObject const& _input, TestSuite::TestSuiteOptions& _opt)
{
    DataObject tests;
    if (_opt.doFilling)
    {
    }
    else
    {
        BlockchainTest test(_input);
        for (BlockchainTestInFilled const& bcTest : test.tests())
        {
            // Select test by name if --singletest and --singlenet is set
            if (Options::get().singleTest)
            {
                if (!Options::get().singleSubTestName.empty() && bcTest.testName() != Options::get().singleSubTestName)
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

    /*
    DataObject tests;
    // A blockchain test file contains many tests in one .json file
    for (auto const& i : _input.getSubObjects())
    {
        string const& testname = i.getKey();
        TestOutputHelper::get().setCurrentTestName(testname);

        if (_opt.doFilling)
        {
            {
                TestInfo errorInfo("TestFillerInit");
                TestOutputHelper::get().setCurrentTestInfo(errorInfo);
            }
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

            RunTest(i, _opt);
        }
    }
    */

    return tests;
}
}  // namespace test
