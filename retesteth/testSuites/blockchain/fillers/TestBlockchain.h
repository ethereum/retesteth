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
    TestBlockchain(RPCSession& _session, std::string const& _network)
      : m_session(_session), m_network(_network)
    {
        TestBlock genesisBlock;
        genesisBlock.setNextBlockForked(mineNextBlockAndRewert());
        m_blocks.push_back(genesisBlock);  // Empty genesis
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

    RPCSession& m_session;
    std::string m_sDebugString;       // Debug info of block numbers
    std::string m_chainName;          // Name of this chain
    std::vector<TestBlock> m_blocks;  // List of blocks
    // std::vector<TestBlock> m_knownBlocks;       // List of fork block RLPs
    std::string m_network;                      // Forkname in genesis
};
