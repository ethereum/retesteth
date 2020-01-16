#pragma once
#include "TestBlockchain.h"
#include <ethObjects/common.h>
#include <retesteth/RPCSession.h>
#include <string>
#include <vector>

// TestBlockchain class with chain reorgs
class TestBlockchainManager
{
public:
    TestBlockchainManager(RPCSession& _session, std::string const& _network)
      : m_sCurrentChainName("default"), m_session(_session)
    {
        mapOfKnownChain.emplace(m_sCurrentChainName, TestBlockchain(_network));
    }

    // Perform block generation logic by parsing the filler file section
    void parseBlockFromJson(test::scheme_blockchainTestFiller::blockSection const& _block);

private:
    // Reorg chains on the client if needed (for new block generation)
    void reorgChains(std::string const& _newBlockChainName, size_t _newBLockNumber);

    // Prepare uncles
    vectorOfSchemeBlock prepareUncles(blockSection const& _block, string const& _debug);

    // Parse uncle section in block and generate uncles from all the chain information
    test::scheme_block prepareUncle(
        scheme_uncleHeader _uncleOverwrite, vectorOfSchemeBlock const& _currentBlockPreparedUncles);

    RPCSession& m_session;
    std::string m_sCurrentChainName;                        // Chain name that is mining blocks
    std::map<std::string, TestBlockchain> mapOfKnownChain;  // Memory of test chains
};
