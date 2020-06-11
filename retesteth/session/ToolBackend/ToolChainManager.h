#pragma once
#include "ToolChain.h"
#include <retesteth/testStructures/types/Ethereum/EthereumBlock.h>
#include <retesteth/testStructures/types/RPC/EthGetBlockBy.h>
#include <retesteth/testStructures/types/RPC/SetChainParamsArgs.h>
#include <boost/filesystem.hpp>
namespace fs = boost::filesystem;

namespace toolimpl
{
// Manage test blockchains for reorg
// Manage pending block for eth_sendRawTransaction
// Imports raw blocks (RLP) into apropriate chain, reorg if needed
class ToolChainManager : public GCP_SPointerBase
{
public:
    ToolChainManager(SetChainParamsArgs const& _config, fs::path const& _toolPath);
    void addPendingTransaction(Transaction const& _tr) { m_pendingBlock.getContent().addTransaction(_tr); }
    ToolChain const& currentChain() const { return m_chains.at(m_currentChain).getCContent(); }
    void mineBlocks(size_t _number);

    EthereumBlock const& lastBlock() const { return m_chains.at(m_currentChain).getCContent().lastBlock(); }

    // Construct RPC response
    EthGetBlockBy getBlockByNumber(VALUE const& _blockNumber) const;


private:
    ToolChainManager() {}
    ToolChain& latestChain() { return m_chains.at(m_currentChain).getContent(); }
    EthGetBlockBy internalConstructResponseBlock(EthereumBlock const& _block) const;

    std::map<size_t, spToolChain> m_chains;
    size_t m_currentChain;
    spEthereumBlock m_pendingBlock;
};

}  // namespace toolimpl
