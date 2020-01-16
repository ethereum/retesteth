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
    {}

    void generateBlock(blockSection const& _block, vectorOfSchemeBlock const& _uncles);

    // Remember new block
    void registerBlockRLP(std::string const& _blockRLP) { m_blockRlps.push_back(_blockRLP); }

    // Restore this chain on remote client up to < _number block
    // Restore chain up to _number of blocks. if _number is 0 restore the whole chain
    void restoreUpToNumber(RPCSession& _session, size_t _number, bool _samechain = false);

    void importKnownBlocks(RPCSession& _session) const;
    std::vector<std::string> const& getBlockRlps() const { return m_blockRlps; }
    vectorOfSchemeBlock const& getUnpreparedUncles() const { return m_uncles; }
    vectorOfSchemeBlock& getUnpreparedUnclesUnsafe() { return m_uncles; }
    std::map<size_t, vectorOfSchemeBlock> const& getMapBlocksToPreparedUncles() const
    {
        return m_mapBlocksToPreparedUncles;
    }
    std::map<size_t, vectorOfSchemeBlock>& getMapBlocksToPreparedUnclesUnsafe()
    {
        return m_mapBlocksToPreparedUncles;
    }
    std::string const& getChainName() const { return m_chainName; }

    // Prepare errorinfo about block number and chain name
    string prepareDebugInfoString(std::string const& _newBlockChainName) const;

private:
    // Ask remote client to generate a blockheader that will later used for uncles
    test::scheme_block prepareUnclePopulate();

    // Import transactions on remote client, return prepared json data for test
    DataObject importTransactions(blockSection const& _block);

    void mineBlock(blockSection const& _block);

    RPCSession& m_session;
    std::string m_sDebugString;       // Debug info of block numbers
    std::string m_chainName;          // Name of this chain
    std::vector<TestBlock> m_blocks;  // List of blocks

    std::vector<std::string> m_blockRlps;       // List of Imported block RLPs
    std::vector<std::string> m_knownBlockRlps;  // List of fork block RLPs
    std::string m_network;                      // Forkname in genesis
    std::map<size_t, vectorOfSchemeBlock> m_mapBlocksToPreparedUncles;  // prepared uncles for each
                                                                        // block
};
