#include "BlockchainTestFillerLogic.h"
#include "TestBlockchainManager.h"

namespace test
{
/// Generate blockchain test from filler
void FillTest(scheme_blockchainTestFiller const& _testObject, string const& _network,
    TestSuite::TestSuiteOptions const&, DataObject& _testOut)
{
    // Construct filled blockchain test
    _testOut["sealEngine"] = _testObject.getSealEngine();
    _testOut["network"] = _network;
    _testOut["pre"] = _testObject.getPre().getData();
    if (_testObject.getData().count("_info"))
        _testOut["_info"] = _testObject.getData().atKey("_info");

    RPCSession& session = RPCSession::instance(TestOutputHelper::getThreadID());

    // Initialise chain manager
    ETH_LOGC("FILL GENESIS INFO: ", 6, LogColor::LIME);
    TestBlockchainManager testchain(session, _testObject, _network);
    TestBlock const& genesis = testchain.getLastBlock();
    _testOut["genesisBlockHeader"] = genesis.getDataForTest().atKey("blockHeader");
    _testOut["genesisRLP"] = genesis.getDataForTest().atKey("rlp");

    size_t blocks = 0;
    for (auto const& block : _testObject.getBlocks())
    {
        // Debug
        if (Options::get().blockLimit != 0 && blocks++ >= Options::get().blockLimit)
            break;

        // Generate a test block from test block section
        // With all the information about sidechains and uncles
        testchain.parseBlockFromJson(block, _testObject.getTotalUncleCount() > 0);

        // Get the json output of a constructed block for the test (includes rlp)
        _testOut["blocks"].addArrayObject(testchain.getLastBlock().getDataForTest());
    }

    // Import blocks that have been rewinded with the chain switch
    // Now makes some block invalid. Because block can be mined as valid on side chain
    testchain.syncOnRemoteClient(_testOut["blocks"]);

    // Fill info about the lastblockhash
    scheme_block latestBlock = session.eth_getBlockByNumber(session.eth_blockNumber(), false);
    scheme_state remoteState = getRemoteState(session, latestBlock);
    if (remoteState.isHash())
    {
        compareStates(_testObject.getExpectSection().getExpectSectionFor(_network).getExpectState(),
            session, latestBlock);
    }
    else
        compareStates(_testObject.getExpectSection().getExpectSectionFor(_network).getExpectState(),
            remoteState);

    if (remoteState.isHash())
        _testOut["postStateHash"] = remoteState.getData();
    else
        _testOut["postState"] = remoteState.getData();
    _testOut["lastblockhash"] = latestBlock.getBlockHash();
}

}  // namespace test
