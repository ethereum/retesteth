#pragma once
#include "TestBlock.h"
#include <ethObjects/common.h>
#include <retesteth/RPCSession.h>
#include <string>
#include <vector>

typedef test::scheme_blockchainTestFiller::blockSection blockSection;

// Single blockchain with block rlps
class TestBlockchain
{
public:
    enum class RegenerateGenesis
    {
        TRUE,
        FALSE
    };
    TestBlockchain(RPCSession& _session, scheme_blockchainTestFiller const& _testObject,
        std::string const& _network, RegenerateGenesis _regenerateGenesis)
      : m_session(_session), m_testObject(_testObject), m_network(_network)
    {
        TestBlock genesisBlock;
        if (_regenerateGenesis == RegenerateGenesis::TRUE)
        {
            resetChainParams();
            test::scheme_block latestBlock = _session.eth_getBlockByNumber(BlockNumber("0"), false);
            DataObject& blockTestData = genesisBlock.getDataForTestUnsafe();
            blockTestData["blockHeader"] = latestBlock.getBlockHeader();
            blockTestData["blockHeader"].removeKey("transactions");
            blockTestData["blockHeader"].removeKey("uncles");
            blockTestData["rlp"] = latestBlock.getBlockRLP();
        }

        genesisBlock.setNextBlockForked(mineNextBlockAndRewert());
        m_blocks.push_back(genesisBlock);
    }

    void resetChainParams() const
    {
        DataObject genesisObject =
            m_testObject.getGenesisForRPC(m_network, m_testObject.getSealEngine());
        m_session.test_setChainParams(genesisObject.asJson());
    }

    void generateBlock(
        blockSection const& _block, vectorOfSchemeBlock const& _uncles, bool _generateUncles);

    // Restore this chain on remote client up to < _number block
    // Restore chain up to _number of blocks. if _number is 0 restore the whole chain
    void restoreUpToNumber(RPCSession& _session, size_t _number, bool _samechain);

    std::vector<TestBlock> const& getBlocks() const { return m_blocks; }

    std::string const& getChainName() const { return m_chainName; }

    // Prepare errorinfo about block number and chain name
    string prepareDebugInfoString(std::string const& _newBlockChainName);

    string const& getNetwork() const { return m_network; }

    // Verify post-import exceptin according to expectException section in test
    // Return true if block is valid, false if block is not valid
    bool checkBlockException(string const& _sBlockException) const;

private:
    // Ask remote client to generate a blockheader that will later used for uncles
    test::scheme_block mineNextBlockAndRewert();

    // Import transactions on remote client, return prepared json data for test
    DataObject importTransactions(blockSection const& _block);

    // Mine the test block on remote client.
    // if blockheader is tweaked or there are uncles, postmine tweak this and reimport
    test::scheme_block mineBlock(
        blockSection const& _block, vectorOfSchemeBlock const& _preparedUncleBlocks);

    // After test_mineBlock we can change the blockheader or add uncles. that will require to tweak
    // the block And reimport it again, then check exceptions
    test::scheme_block postmineBlockHeader(blockSection const& _block,
        BlockNumber const& _latestBlockNumber, std::vector<scheme_block> const& _uncles);

    RPCSession& m_session;                            // Session with the client
    scheme_blockchainTestFiller const& m_testObject;  // Test data information
    std::string m_network;                            // Forkname in genesis

    std::string m_sDebugString;       // Debug info of block numbers
    std::string m_chainName;          // Name of this chain
    std::vector<TestBlock> m_blocks;  // List of blocks
    // std::vector<TestBlock> m_knownBlocks;       // List of fork block RLPs
};
