#include "TestBlockchainManager.h"
#include <retesteth/TestSuite.h>
#include <retesteth/testStructures/types/BlockchainTests/BlockchainTestFiller.h>
#include <retesteth/testSuites/Common.h>

using namespace blockchainfiller;
namespace test
{
/// Generate blockchain test from filler
DataObject FillTest(BlockchainTestInFiller const& _test, TestSuite::TestSuiteOptions const& _opt)
{
    (void)_opt;
    DataObject result;
    SessionInterface& session = RPCSession::instance(TestOutputHelper::getThreadID());
    for (FORK const& net : _test.getAllForksFromExpectSections())
    {
        for (auto const& expect : _test.expects())
        {
            // if expect is for this network, generate the test
            if (expect.forks().count(net))
            {
                // Construct filled blockchain test
                DataObject filledTest;
                string const newtestname = _test.testName() + "_" + net.asString();
                TestOutputHelper::get().setCurrentTestName(newtestname);
                filledTest.setKey(newtestname);
                if (_test.hasInfo())
                    filledTest["_info"]["comment"] = _test.Info().comment();
                filledTest["pre"] = _test.Pre().asDataObject();
                filledTest["sealEngine"] = sealEngineToStr(_test.sealEngine());
                filledTest["network"] = net.asString();

                // Initialise chain manager
                ETH_LOGC("FILL GENESIS INFO: ", 6, LogColor::LIME);
                TestBlockchainManager testchain(_test.Env(), _test.Pre(), _test.sealEngine(), net);
                TestBlock const& genesis = testchain.getLastBlock();
                filledTest["genesisBlockHeader"] = genesis.ethBlock().header().asDataObject();
                filledTest["genesisRLP"] = genesis.ethBlock().getRLP().asString();

                TestOutputHelper::get().setUnitTestExceptions(_test.unitTestExceptions());

                size_t blocks = 0;
                for (auto const& block : _test.blocks())
                {
                    // Debug
                    if (Options::get().blockLimit != 0 && blocks++ >= Options::get().blockLimit)
                        break;

                    // Generate a test block from filler block section
                    // Asks remote client to generate all the uncles and hashes for it
                    testchain.parseBlockFromFiller(block, _test.hasUnclesInTest());

                    // If block is not disabled for testing purposes
                    // Get the json output of a constructed block for the test (includes rlp)
                    if (!testchain.getLastBlock().isDoNotExport())
                        filledTest["blocks"].addArrayObject(testchain.getLastBlock().asDataObject());
                }

                // Import blocks that have been rewinded with the chain switch
                // This makes some block invalid. Because block can be mined as valid on side chain
                // So just import all block ever generated with test filler
                testchain.syncOnRemoteClient(filledTest["blocks"]);

                // Fill info about the lastblockhash
                EthGetBlockBy finalBlock(session.eth_getBlockByNumber(session.eth_blockNumber(), Request::LESSOBJECTS));

                State remoteState = getRemoteState(session);
                if (Options::get().fullstate)
                    compareStates(expect.result(), remoteState);
                else
                    compareStates(expect.result(), session);

                //_testOut["postStateHash"] = remoteState.getData();
                filledTest["postState"] = remoteState.asDataObject();
                filledTest["lastblockhash"] = finalBlock.header().hash().asString();
                result.addSubObject(filledTest);

            }  // expects count net
        }
    }
    return result;
}

}  // namespace test
