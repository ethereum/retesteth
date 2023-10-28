#pragma once
#include "TestBlockchain.h"
#include <retesteth/session/Session.h>
#include <retesteth/testStructures/types/BlockchainTests/BlockchainTestFiller.h>
#include <string>
#include <vector>

namespace test::blockchainfiller
{
// TestBlockchain class with chain reorgs
class TestBlockchainManager
{
public:
    // Initialize blockchain manager with first chain information
    // _env, _pre, _engine, _network
    TestBlockchainManager(
        BlockchainTestFillerEnv const& _genesisEnv, State const& _genesisPre, SealEngine _engine, FORK const& _network);

    // Perform block generation logic by parsing the filler file section
    std::vector<spDataObject> parseBlockFromFiller(BlockchainTestFillerBlock const& _block, bool _generateUncles);

    // Return the last block from the current chain (not the top of all chains)
    TestBlock const& getLastBlock();

    // Import all generated blocks at the same order as they are in tests
    void syncOnRemoteClient(DataObject& _exportBlocksSection) const;

    void performOptionCommandsOnGenesis();

private:
    std::vector<spDataObject> _generateBlocksFromFillerTestBlock(BlockchainTestFillerBlock const&, vectorOfSchemeBlock const&, bool _generateUncles);
    void _makeTheFilledBlockFromFiller(BlockchainTestFillerBlock const&, vectorOfSchemeBlock const&, bool _generateUncles);

private:
    // Reorg chains on the client if needed for _newBlock that potentially comes from another chain
    void reorgChains(BlockchainTestFillerBlock const& _newBlock);

    // Prepare uncles
    vectorOfSchemeBlock prepareUncles(BlockchainTestFillerBlock const& _block, std::string const& _debug);

    // Parse uncle section in block and generate uncles from all the chain information
    spBlockHeader prepareUncle(
        BlockchainTestFillerUncle _uncleOverwrite, vectorOfSchemeBlock const& _currentBlockPreparedUncles);

    session::SessionInterface& m_session;  // session with the client

    std::string m_sCurrentChainName;        // Chain name that is mining blocks
    std::string const m_sDefaultChainName;  // Default chain name to restore genesis

    BlockchainTestFillerEnv const& m_genesisEnv;  // Genesis Info
    State m_genesisPre;
    SealEngine m_sealEngine;


    FORK const m_sDefaultChainNet;  // First defined blockchain network
    bool m_wasAtLeastOneFork;       // Wether we swithced chains on remote

    typedef std::tuple<BYTES, std::string> RLPAndException;
    std::vector<RLPAndException> m_testBlockRLPs;  // List of prepared blocks exactly as they are
                                                   // defined in test (order)

    TestBlockchain& getDefaultChain();
    TestBlockchain& getCurrentChain();
    std::map<std::string, TestBlockchain> m_mapOfKnownChain;  // Memory of test chains
};

}  // namespace blockchainfiller
