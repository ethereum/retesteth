#include "TestBlockchainManager.h"
#include <retesteth/ExitHandler.h>
#include <retesteth/Options.h>
#include <retesteth/testStructures/types/BlockchainTests/BlockchainTestFiller.h>
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
    ETH_DC_MESSAGE(DC::TESTLOG, "Filling " + _test.testName());

    spDataObject result;
    if (ExitHandler::receivedExitSignal())
        return result;
    SessionInterface& session = RPCSession::instance(TestOutputHelper::getThreadID());
    for (FORK const& net : _test.getAllForksFromExpectSections())
    {
        if (!Options::get().singleTestNet.empty() && net.asString() != Options::get().singleTestNet)
            continue;

        for (auto const& expect : _test.expects())
        {
            // if expect is for this network, generate the test
            if (expect.hasFork(net))
            {
                // Construct filled blockchain test
                spDataObject _filledTest;
                DataObject& filledTest = _filledTest.getContent();
                string const newtestname = _test.testName() + "_" + net.asString();
                TestOutputHelper::get().setCurrentTestName(newtestname);
                filledTest.setKey(newtestname);
                if (_test.hasInfo())
                    (*_filledTest).atKeyPointer("_info") = _test.Info().rawData();
                filledTest["sealEngine"] = sealEngineToStr(_test.sealEngine());
                filledTest["network"] = net.asString();
                filledTest.atKeyPointer("pre") = _test.Pre().asDataObject();

                // Initialise chain manager
                ETH_DC_MESSAGEC(DC::TESTLOG, "FILL GENESIS INFO: ", LogColor::LIME);
                TestBlockchainManager testchain(_test.Env(), _test.Pre(), _test.sealEngine(), net);
                TestBlock const& genesis = testchain.getLastBlock();
                filledTest.atKeyPointer("genesisBlockHeader") = genesis.getTestHeader()->asDataObject();
                filledTest["genesisRLP"] = genesis.getRawRLP().asString();

                TestOutputHelper::get().setUnitTestExceptions(_test.unitTestExceptions());

                size_t blocks = 0;
                for (auto const& block : _test.blocks())
                {
                    if (ExitHandler::receivedExitSignal())
                        return _filledTest;
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

                try
                {
                    State remoteState(getRemoteState(session));
                    compareStates(expect.result(), remoteState);
                    filledTest.atKeyPointer("postState") = remoteState.asDataObject();
                    if (Options::get().poststate)
                        ETH_DC_MESSAGE(DC::STATE, "\nState Dump:" + TestOutputHelper::get().testInfo().errorDebug() + cDefault +
                                                      " \n" + filledTest.atKey("postState").asJson());
                }
                catch (StateTooBig const&)
                {
                    compareStates(expect.result(), session);
                    filledTest["postStateHash"] = finalBlock.header()->stateRoot().asString();
                }

                if (Options::get().poststate)
                    ETH_DC_MESSAGE(DC::STATE, "PostState " + TestOutputHelper::get().testInfo().errorDebug() + " : \n" +
                                                  cDefault + "Hash: " + finalBlock.header()->stateRoot().asString());


                filledTest["lastblockhash"] = finalBlock.header()->hash().asString();
                (*result).addSubObject(_filledTest);

                verifyFilledTest(_test.unitTestVerify(), _filledTest, net);
            }  // expects count net
        }
    }

    return result;
}
}  // namespace test
