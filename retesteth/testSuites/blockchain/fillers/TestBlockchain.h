#pragma once
#include "TestBlock.h"
#include <retesteth/TestOutputHelper.h>
#include <retesteth/session/Session.h>
#include <retesteth/testStructures/Common.h>
#include <string>
#include <vector>

namespace test::blockchainfiller
{
// Used to tell remote client to reset chain params on another network
enum class RegenerateGenesis
{
    TRUE,
    FALSE
};


class TestBlockchain
{
public:
    // Single blockchain with block rlps, keep track of a blockchain information
    // Using _env, _pre, _engine, _network settings
    TestBlockchain(BlockchainTestFillerEnv const& _testEnv, State const& _genesisState, SealEngine _engine,
        FORK const& _network, std::string const& _chainName, RegenerateGenesis _regenerateGenesis);

    // Need to call resetChainParams because TestBLockchainManager could have chains with different networks
    void resetChainParams() const;

    void generateBlock(BlockchainTestFillerBlock const& _block, vectorOfSchemeBlock const& _uncles, bool _generateUncles);

    // Restore this chain on remote client up to < _number block
    // Restore chain up to _number of blocks. if _number is 0 restore the whole chain
    void restoreUpToNumber(session::SessionInterface& _session, VALUE const& _number, bool _samechain);

    std::vector<TestBlock> const& getBlocks() const { return m_blocks; }

    std::string const& getChainName() const { return m_chainName; }

    // Prepare errorinfo about block number and chain name
    std::string prepareDebugInfoString(std::string const& _newBlockChainName);

    FORK const& getNetwork() const { return m_network; }

    // Verify post-import exceptin according to expectException section in test
    // Return true if block is valid, false if block is not valid
    static bool checkBlockException(session::SessionInterface const& _session, std::string const& _sBlockException);

private:
    // Ask remote client to generate a blockheader that will later used for uncles
    spBlockHeader mineNextBlockAndRevert();

    // Mine the test block on remote client.
    // if blockheader is tweaked or there are uncles, postmine tweak this and reimport
    GCP_SPointer<EthGetBlockBy> mineBlock(
        BlockchainTestFillerBlock const& _block, vectorOfSchemeBlock const& _preparedUncleBlocks, BYTES& _rawRLP);

    // After test_mineBlock we can change the blockheader or add uncles. that will require to tweak
    // the block And reimport it again, then check exceptions
    FH32 postmineBlockHeader(BlockchainTestFillerBlock const& _block, VALUE const& _latestBlockNumber,
        std::vector<spBlockHeader> const& _uncles, BYTES& _rawRLP);

    session::SessionInterface& m_session;             // Session with the client
    FORK m_network;                                   // Forkname in genesis
    SealEngine m_sealEngine;                          // Chain seal engine information
    BlockchainTestFillerEnv const& m_testEnv;         // Chain genesis data information
    State const& m_genesisState;                      // PreState of genesis block


    std::string m_sDebugString;       // Debug info of block numbers
    std::string m_chainName;          // Name of this chain
    std::vector<TestBlock> m_blocks;  // List of blocks
    // std::vector<TestBlock> m_knownBlocks;       // List of fork block RLPs
};

}  // namespace test::blockchainfiller
