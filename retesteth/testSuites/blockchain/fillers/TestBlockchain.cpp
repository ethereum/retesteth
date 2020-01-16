#include "TestBlockchain.h"

void TestBlockchain::generateBlock(blockSection const& _block, vectorOfSchemeBlock const& _uncles)
{
    TestBlock newBlock;

    // Ask remote client to generate a blockheader that will later used for uncles
    newBlock.setUnpreparedUncle(prepareUnclePopulate());

    // Prepare transactions for the block
    newBlock.getDataForTest()["transactions"] = importTransactions(_block);

    // Put prepared uncles into dataobject
    for (auto const& uncle : _uncles)
        newBlock.getDataForTest()["uncleHeaders"].addArrayObject(uncle.getBlockHeader());

    mineBlock(_block);
}

void TestBlockchain::mineBlock(blockSection const& _block)
{
    ETH_LOGC("MINE BLOCK: " + m_sDebugString, 6, LogColor::YELLOW);
    string latestBlockNumber = m_session.test_mineBlocks(1);
    bool isUnclesInTest = _block.getData().count("uncleHeaders") ?
                              _block.getData().atKey("uncleHeaders").getSubObjects().size() > 0 :
                              false;

    if (_block.getData().count("blockHeader") || isUnclesInTest)
    {
        ETH_LOG("Postmine blockheader: " + m_sDebugString, 6);
        test::scheme_block latestBlock = postmineBlockHeader(
            m_session, _block, latestBlockNumber, m_network, preparedUncleBlocks);
        if (!latestBlock.isValid())
        {
            blockSection.removeKey("transactions");
            blockSection.removeKey("uncleHeaders");
        }
    }
    else
        latestBlock = session.eth_getBlockByNumber(latestBlockNumber, true);
}

// Import transactions on remote client
DataObject TestBlockchain::importTransactions(blockSection const& _block)
{
    DataObject transactionsArray = DataObject(DataType::Array);
    ETH_LOGC("Import transactions: " + m_sDebugString, 6, LogColor::YELLOW);
    for (auto const& tr : _block.getTransactions())
    {
        m_session.eth_sendRawTransaction(tr.getSignedRLP());
        transactionsArray.addArrayObject(tr.getDataForBCTest());
    }
    return transactionsArray;
}

// Ask remote client to generate a blockheader that will later used for uncles
test::scheme_block TestBlockchain::prepareUnclePopulate()
{
    ETH_LOGC("Populate clean uncle from: " + m_sDebugString, 6, LogColor::YELLOW);
    string latestBlockNumber = m_session.test_mineBlocks(1);
    test::scheme_block uncle = m_session.eth_getBlockByNumber(latestBlockNumber, false);
    m_session.test_rewindToBlock(m_knownBlockRlps.size());  // rewind to the previous block
    m_session.test_modifyTimestamp(1000);  // Shift block timestamp relative to previous block
    ETH_LOGC("Finish populate clean uncle from: " + m_sDebugString, 6, LogColor::YELLOW);
    return uncle;
}

string TestBlockchain::prepareDebugInfoString(string const& _newBlockChainName) const
{
    string sBlockNumber;
    size_t newBlockNumber = m_blocks.size();
    TestInfo errorInfo(m_network, newBlockNumber, _newBlockChainName);
    if (Options::get().logVerbosity >= 6)
        sBlockNumber = toString(newBlockNumber);  // very heavy
    TestOutputHelper::get().setCurrentTestInfo(errorInfo);
    m_sDebugString = "(bl: " + sBlockNumber + ", ch: " + _newBlockChainName + ")";
    ETH_LOGC("Generating a test block: " + m_sDebugString, 6, LogColor::YELLOW);
    return m_sDebugString;
}
