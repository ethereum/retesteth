#include "BlockchainTestLogic.h"
#include "BlockchainTestRunner.h"
#include <retesteth/EthChecks.h>
#include <retesteth/ExitHandler.h>
#include <retesteth/Options.h>
#include <retesteth/helpers/TestOutputHelper.h>
using namespace std;
using namespace test;
using namespace test::debug;
using namespace test::session;

namespace test
{

/// Read and execute the test from the file
void RunTest(BlockchainTestInFilled const& _test, TestSuite::TestSuiteOptions const& _opt)
{
    if (!BlockchainTestRunner::validateFork(_test.testName(), _test.network()))
        return;

    BlockchainTestRunner runner(_test, _opt);
    if (runner.checkBigIntSkip())
        return;
    runner.setChainParams();
    runner.performOptionCommandsOnGenesis();

    for (BlockchainTestBlock const& tblock : _test.blocks())
    {
        CHECKEXIT
        if (runner.abortBlock())
            break;

        runner.incrementBlockAndSetTestInfo();
        runner.validateTransactionSequence(tblock);
        runner.validateRlpDecodedInInvalidBlocks(tblock);

        auto const blHash = runner.mineBlock(tblock.rlp());
        if (runner.checkLastRPCBlockException(tblock))
            continue;

        auto const blockFull = runner.requestBlock(blHash);
        runner.performOptionCommands(tblock, blockFull);
        runner.validateBlockHeader(tblock, blockFull);
        runner.validateUncles(tblock, blockFull);
        runner.validateTransactions(tblock, blockFull);
    }

    auto const lastBlockLess = runner.requestBlock(runner.session().eth_blockNumber());
    runner.checkPostState(lastBlockLess);
    runner.checkGenesis();
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
        if (Options::get().getvectors)
        {
            test.registerAllVectors();
            return tests;
        }
        ETH_DC_MESSAGE(DC::TESTLOG, "Parse test done");

        for (BlockchainTestInFilled const& bcTest : test.tests())
        {
            // Select test by name if --singletest and --singlenet is set
            if (Options::get().singletest.initialized())
            {
                // If we run python test from .py do not select by subtest
                if (!TestOutputHelper::get().getPythonTestFlag())
                {
                    if (!Options::get().singletest.subname.empty()
                        && bcTest.testName() != Options::get().singletest.subname)
                        continue;
                }
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
