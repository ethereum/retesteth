#include "TestBlockchainManager.h"
#include "BlockchainTestFillerRunner.h"
#include <retesteth/ExitHandler.h>
#include <retesteth/Options.h>
#include <retesteth/testSuiteRunner/TestSuite.h>
#include <retesteth/testSuites/Common.h>

using namespace std;
using namespace dataobject;
using namespace test::session;
using namespace test::debug;
using namespace test::blockchainfiller;
namespace test
{

/// Generate blockchain test from filler
spDataObject FillTest(BlockchainTestInFiller const& _test, TestSuite::TestSuiteOptions const& _opt)
{
    (void)_opt;
    spDataObject result;
    BlockchainTestFillerRunner filler(_test);
    CHECKEXITR(result);

    auto const allForks = _test.getAllForksFromExpectSections();
    if (hasSkipFork(allForks))
        return spDataObject(new DataObject(DataType::Null));

    for (FORK const& net : allForks)
    {
        CHECKEXITR(result)
        if (networkSkip(net, _test.testName()))
            continue;

        for (auto const& expect : _test.expects())
        {
            // if expect is for this network, generate the test
            if (expect.hasFork(net))
            {
                auto filledTest = filler.makeNewBCTestForNet(net);
                auto testchain = filler.makeTestChainManager(net);

                filler.makeGenesis(filledTest, testchain);
                filler.setTestInfoAndExpectExceptions(net);

                testchain.performOptionCommandsOnGenesis();

                size_t blockNumber = 0;
                for (auto const& block : _test.blocks())
                {
                    CHECKEXITR(result)

                    if (filler.optionsLimitBlock(blockNumber++))
                        break;

                    // Generate a test block from filler block section
                    // Asks remote client to generate all the uncles and hashes for it
                    std::vector<spDataObject> constructedBlocks = testchain.parseBlockFromFiller(block, _test.hasUnclesInTest());
                    for (auto const& blockJson : constructedBlocks)
                        (*filledTest)["blocks"].addArrayObject(blockJson);
                }

                // Import blocks that have been rewinded with the chain switch
                // This makes some block invalid. Because block can be mined as valid on side chain
                // So just import all block ever generated with test filler
                testchain.syncOnRemoteClient((*filledTest)["blocks"]);

                EthGetBlockBy finalBlock = filler.getLastBlock();

                filler.performOptionsOnFinalState(finalBlock);
                filler.compareFinalState(filledTest, expect.result(), finalBlock);

                (*result).addSubObject(filledTest);

                verifyFilledTest(_test.unitTestVerify(), filledTest, net);
                for (auto const& ex : TestOutputHelper::get().getUnitTestExceptions())
                    ETH_FAIL_MESSAGE("Expected exception didn't occur: \n`" + ex + "`");

            }  // expects count net
        }
    }

    return result;
}
}  // namespace test
