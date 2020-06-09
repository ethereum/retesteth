#pragma once
#include "ToolChain.h"
#include <retesteth/testStructures/types/Ethereum/EthereumBlock.h>
#include <retesteth/testStructures/types/RPC/SetChainParamsArgs.h>
#include <boost/filesystem.hpp>
namespace fs = boost::filesystem;

namespace toolimpl
{
// Manage test blockchains for reorg
class ToolChainManager : public GCP_SPointerBase
{
public:
    ToolChainManager(SetChainParamsArgs const& _config, fs::path const& _toolPath);
    void addPendingTransaction(Transaction const& _tr) { m_pendingBlock.getContent().addTransaction(_tr); }

private:
    ToolChainManager() {}
    std::map<size_t, spToolChain> m_chains;
    size_t m_currentChain;
    spEthereumBlock m_pendingBlock;
};

}  // namespace toolimpl
