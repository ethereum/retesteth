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
    DataObject genesisObject = _testObject.getGenesisForRPC(_network, _testObject.getSealEngine());
    session.test_setChainParams(genesisObject.asJson());

    ETH_LOGC("FILL GENESIS INFO: ", 6, LogColor::LIME);
    test::scheme_block latestBlock = session.eth_getBlockByNumber(BlockNumber("0"), false);
    _testOut["genesisBlockHeader"] = latestBlock.getBlockHeader();
    _testOut["genesisBlockHeader"].removeKey("transactions");
    _testOut["genesisBlockHeader"].removeKey("uncles");
    _testOut["genesisRLP"] = latestBlock.getBlockRLP();

    size_t blocks = 0;
    TestBlockchainManager testchain(session, _network);
    for (auto const& block : _testObject.getBlocks())
    {
        if (Options::get().blockLimit != 0 && blocks++ >= Options::get().blockLimit)
            break;
        testchain.attemptToSetCurrentChainName(block.getChainName());
        ETH_LOGC("STARTING A NEW BLOCK: ", 6, LogColor::LIME);
        // Generate a test block from test block section
        // With all the information about sidechains and uncles
        testchain.parseBlockFromJson(block, _testObject.getTotalUncleCount() > 0);
        _testOut["blocks"].addArrayObject(testchain.getLastBlock().getDataForTest());
    }

    // Import blocks that have been rewinded with the chain switch
    testchain.syncOnRemoteClient();

    // Fill info about the lastblockhash
    latestBlock = session.eth_getBlockByNumber(session.eth_blockNumber(), false);
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

}  // namespace test
