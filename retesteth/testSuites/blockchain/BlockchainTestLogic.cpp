#include "BlockchainTestLogic.h"
#include "fillers/BlockchainTestFillerLogic.h"
#include <retesteth/EthChecks.h>
#include <retesteth/session/RPCSession.h>
#include <retesteth/testSuites/Common.h>


namespace test
{

/// Read and execute the test from the file
void RunTest(DataObject const& _testObject, TestSuite::TestSuiteOptions const& _opt)
{
    {
        TestInfo errorInfo("RunTestInit");
        TestOutputHelper::get().setCurrentTestInfo(errorInfo);
    }
    if (Options::get().logVerbosity > 1)
        std::cout << "Running " << TestOutputHelper::get().testName() << std::endl;
    scheme_blockchainTest inputTest(_testObject, _opt.isLegacyTests);
    TestOutputHelper::get().setUnitTestExceptions(inputTest.getUnitTestExceptions());
    SessionInterface& session = RPCSession::instance(TestOutputHelper::getThreadID());

    // Info for genesis
    TestInfo errorInfo (inputTest.getNetwork(), 0);
    TestOutputHelper::get().setCurrentTestInfo(errorInfo);
    session.test_setChainParams(
        inputTest.getGenesisForRPC(inputTest.getNetwork(), inputTest.getEngine()).asJson());

    // for all blocks
    size_t blockNumber = 0;
    for (auto const& bdata : inputTest.getBlocks())
    {
        TestInfo errorInfo(inputTest.getNetwork(), blockNumber++);
        TestOutputHelper::get().setCurrentTestInfo(errorInfo);
        string const blHash = session.test_importRawBlock(bdata.atKey("rlp").asString());
        if (session.getLastRPCError().type() != DataType::Null)
        {
            if (!_opt.allowInvalidBlocks)
                ETH_ERROR_MESSAGE("Running blockchain test: " + session.getLastRPCError().atKey("message").asString());
        }

        // Check imported block against the fields in test
        if (bdata.count("blockHeader"))
        {
            // Check Blockheader
            DataObject inTestHeader(DataType::Null);
            test::scheme_block latestBlock = session.eth_getBlockByHash(blHash, true);
            bool condition = latestBlock.getBlockHeader() == bdata.atKey("blockHeader");
            if (_opt.isLegacyTests)
            {
                inTestHeader = bdata.atKey("blockHeader");  // copy!!!
                string const extraDataInTest =
                    bdata.atKey("blockHeader").atKey("extraData").asString();
                if ((extraDataInTest == "0x00" || extraDataInTest == "0x") &&
                    (latestBlock.getBlockHeader().atKey("extraData").asString() == "0x00" ||
                        latestBlock.getBlockHeader().atKey("extraData").asString() == "0x"))
                {
                    inTestHeader["extraData"] = latestBlock.getBlockHeader().atKey("extraData");
                }
                if (latestBlock.getBlockHeader().atKey("nonce").asString() == "0x0000000000000000")
                {
                    // because on NoProof mixHash is returned as 0x00 by other client, even if block
                    // imported with mixHash/nonce
                    inTestHeader["mixHash"] =
                        "0x0000000000000000000000000000000000000000000000000000000000000000";
                    inTestHeader["nonce"] = "0x0000000000000000";
                }
                inTestHeader["hash"] = latestBlock.getBlockHeader().atKey("hash");
                condition = latestBlock.getBlockHeader() == inTestHeader;
            }

            string const jsonHeader = inTestHeader.type() == DataType::Null ?
                                          bdata.atKey("blockHeader").asJson() :
                                          "(adjusted)" + inTestHeader.asJson();
            string message = "Client return HEADER: " + latestBlock.getBlockHeader().asJson() +
                             "\n vs \n" + "Test HEADER: " + jsonHeader;
            ETH_ERROR_REQUIRE_MESSAGE(condition,
                "Client report different blockheader after importing the rlp than expected by "
                "test! \n" +
                    message);

            // Verify uncles to one described in the fields
            if (bdata.count("uncleHeaders"))
            {
                size_t ind = 0;
                auto const& testUncleList = bdata.atKey("uncleHeaders").getSubObjects();
                for (auto const& uncle : latestBlock.getUncles())
                {
                    message = "(" + uncle.asString() +
                              " != " + testUncleList.at(ind).atKey("hash").asString() + ")";
                    if (uncle.asString() != testUncleList.at(ind++).atKey("hash").asString())
                        ETH_ERROR_MESSAGE(
                            "Remote client returned block with unclehash that is not expected by "
                            "test! " +
                            message);
                }
            }

            // Check Transaction count
            message =
                "Client return TRANSACTIONS: " + to_string(latestBlock.getTransactionCount()) +
                " vs " + "Test TRANSACTIONS: " +
                to_string(bdata.atKey("transactions").getSubObjects().size());
            ETH_ERROR_REQUIRE_MESSAGE(latestBlock.getTransactionCount() ==
                                          bdata.atKey("transactions").getSubObjects().size(),
                "Client report different transaction count after importing the rlp than expected "
                "by test! \n" +
                    message);

            // Verify transactions to one described in the fields
            size_t ind = 0;
            auto const& testTrList = bdata.atKey("transactions").getSubObjects();

            for (auto const& tr : latestBlock.getTransactions())
            {
                DataObject const& testTr = testTrList.at(ind++);
                ETH_ERROR_REQUIRE_MESSAGE(tr.atKey("blockHash") == blHash,
                    "Error checking remote transaction, remote tr `blockHash` is different to "
                    "requested "
                    "block! (" +
                        tr.atKey("blockHash").asString() + " != " + blHash + ")");

                /*0x1 != 0x01 issue*/
                ETH_ERROR_REQUIRE_MESSAGE(
                    dev::toCompactHexPrefixed(dev::u256(tr.atKey("blockNumber").asString()), 1) ==
                        latestBlock.getBlockHeader().atKey("number").asString(),
                    "Error checking remote transaction, remote tr `blockNumber` is different to "
                    "requested "
                    "block! (" +
                        tr.atKey("blockNumber").asString() +
                        " != " + latestBlock.getBlockHeader().atKey("number").asString() + ")");

                /*0x11 != 0x0011 issue*/
                auto verifyTr = [&tr, &testTr](
                                    string const& aField, string const& bField, size_t length = 1) {
                    bool condition = true;
                    if (length == 0)  // skip conversion
                    {
                        condition = tr.atKey(aField).asString() == testTr.atKey(bField).asString();
                        // transaction data returned by aleth is "" instead of "0x" in blockchain
                        // tests
                        condition = condition || "0x" + tr.atKey(aField).asString() ==
                                                     testTr.atKey(bField).asString();
                    }
                    else
                        condition =
                            dev::toCompactHexPrefixed(dev::u256(tr.atKey(aField).asString()),
                                length) == testTr.atKey(bField).asString();
                    ETH_ERROR_REQUIRE_MESSAGE(
                        condition, "Error checking remote transaction, remote tr `" + aField +
                                       "` is different to test tr `" + bField + "` (" +
                                       tr.atKey(aField).asString() +
                                       " != " + testTr.atKey(bField).asString() + ")");
                };

                verifyTr("input", "data", 0);
                verifyTr("gas", "gasLimit");
                verifyTr("gasPrice", "gasPrice");
                verifyTr("nonce", "nonce");
                verifyTr("v", "v");
                verifyTr("r", "r", _opt.isLegacyTests ? 1 : 32);
                verifyTr("s", "s", _opt.isLegacyTests ? 1 : 32);
                verifyTr("value", "value");
                if (tr.atKey("to").type() != DataType::Null)
                    verifyTr("to", "to", 20);
            }

            // Check uncles count
            if (bdata.count("uncleHeaders"))
            {
                message =
                    "Client return UNCLES: " + to_string(latestBlock.getUncles().size()) + " vs " +
                    "Test UNCLES: " + to_string(bdata.atKey("uncleHeaders").getSubObjects().size());
                ETH_ERROR_REQUIRE_MESSAGE(latestBlock.getUncles().size() ==
                                              bdata.atKey("uncleHeaders").getSubObjects().size(),
                    "Client report different uncle count after importing the rlp than expected by "
                    "test! \n" +
                        message);
            }
        }
    }

    // wait for blocks to process
    // std::this_thread::sleep_for(std::chrono::seconds(10));

    scheme_block latestBlock = session.eth_getBlockByNumber(session.eth_blockNumber(), false);
    if (inputTest.getPost().isHash())
        validatePostHash(session, inputTest.getPost().getHash(), latestBlock);
    else
        compareStates(scheme_expectState(inputTest.getPost().getData()), session, latestBlock);

    if (inputTest.getLastBlockHash() != latestBlock.getBlockHash())
        ETH_ERROR_MESSAGE("lastblockhash does not match! remote: " + latestBlock.getBlockHash() +
                          ", test: " + inputTest.getLastBlockHash());

    if (inputTest.getData().count("genesisRLP"))
    {
        if (_opt.isLegacyTests &&
            Options::getDynamicOptions().getCurrentConfig().getFolderName() == "aleth")
        {
            // skip old generated tests (Legacy) genesis check on aleth
            // because mixHash and nonce return is different to geth
        }
        else
        {
            string const& genesisRLP = inputTest.getData().atKey("genesisRLP").asString();
            latestBlock = session.eth_getBlockByNumber(BlockNumber("0"), false);
            if (latestBlock.getBlockRLP() != genesisRLP)
                ETH_ERROR_MESSAGE("genesisRLP in test != genesisRLP on remote client! (" +
                                  genesisRLP + "' != '" + latestBlock.getBlockRLP() + "'");
        }
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
