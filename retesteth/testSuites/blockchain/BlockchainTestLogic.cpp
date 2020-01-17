#include "BlockchainTestLogic.h"
#include "fillers/BlockchainTestFillerLogic.h"
#include <retesteth/EthChecks.h>
#include <retesteth/RPCSession.h>
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
    scheme_blockchainTest inputTest(_testObject);
    RPCSession& session = RPCSession::instance(TestOutputHelper::getThreadID());

    // Info for genesis
    TestInfo errorInfo (inputTest.getNetwork(), 0);
    TestOutputHelper::get().setCurrentTestInfo(errorInfo);
    session.test_setChainParams(
        inputTest.getGenesisForRPC(inputTest.getNetwork(), inputTest.getEngine()).asJson());

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

    string const& genesisRLP = inputTest.getData().atKey("genesisRLP").asString();
    latestBlock = session.eth_getBlockByNumber(BlockNumber("0"), false);
    if (latestBlock.getBlockRLP() != genesisRLP)
        ETH_ERROR_MESSAGE("genesisRLP in test != genesisRLP on remote client! (" + genesisRLP +
                          "' != '" + latestBlock.getBlockRLP() + "'");
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
