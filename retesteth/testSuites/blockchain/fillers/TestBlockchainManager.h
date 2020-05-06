#pragma once
#include "TestBlockchain.h"
#include <ethObjects/common.h>
#include <retesteth/session/RPCSession.h>
#include <string>
#include <vector>

// TestBlockchain class with chain reorgs
class TestBlockchainManager
{
public:
    TestBlockchainManager(SessionInterface& _session,
        scheme_blockchainTestFiller const& _testObject, std::string const& _network)
      : m_session(_session),
        m_testObject(_testObject),
        m_sDefaultChainName(scheme_blockchainTestFiller::blockSection::getDefaultChainName()),
        m_sDefaultChainNet(_network)
    {
        m_wasAtLeastOneFork = false;
        // m_sCurrentChainName is unknown at this point. the first block of the test defines it
        // but we want genesis to be generated anyway before that
        m_sCurrentChainName = m_sDefaultChainName;
        m_mapOfKnownChain.emplace(
            m_sCurrentChainName, TestBlockchain(m_session, _testObject, _network,
                                     TestBlockchain::RegenerateGenesis::TRUE));
    }

    // Perform block generation logic by parsing the filler file section
    void parseBlockFromJson(blockSection const& _block, bool _generateUncles);

    // Return the last block from the current chain (not the top of all chains)
    TestBlock const& getLastBlock()
    {
        TestBlockchain const& chain = m_mapOfKnownChain.at(m_sCurrentChainName);
        assert(chain.getBlocks().size() > 0);
        return chain.getBlocks().at(chain.getBlocks().size() - 1);
    }

    // Import all generated blocks at the same order as they are in tests
    void syncOnRemoteClient(DataObject& _exportBlocksSection) const;

private:
    // Reorg chains on the client if needed (for new block generation)
    void reorgChains(blockSection const& _block);

    // Prepare uncles
    vectorOfSchemeBlock prepareUncles(blockSection const& _block, string const& _debug);

    // Parse uncle section in block and generate uncles from all the chain information
    test::scheme_RPCBlock prepareUncle(
        scheme_uncleHeader _uncleOverwrite, vectorOfSchemeBlock const& _currentBlockPreparedUncles);

    SessionInterface& m_session;                      // session with the client
    scheme_blockchainTestFiller const& m_testObject;  // testData to generate genesis

    std::string m_sCurrentChainName;        // Chain name that is mining blocks
    std::string const m_sDefaultChainName;  // Default chain name to restore genesis
    std::string const m_sDefaultChainNet;   // First defined blockchain network
    bool m_wasAtLeastOneFork;               // Wether we swithced chains on remote

    typedef std::tuple<std::string, std::string> RLPAndException;
    std::vector<RLPAndException> m_testBlockRLPs;  // List of prepared blocks exactly as they are
                                                   // defined in test (order)
    std::map<std::string, TestBlockchain> m_mapOfKnownChain;  // Memory of test chains
};
