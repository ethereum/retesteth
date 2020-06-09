#include "ToolChainManager.h"

namespace toolimpl
{
ToolChainManager::ToolChainManager(SetChainParamsArgs const& _config, fs::path const& _toolPath)
{
    m_currentChain = 0;
    m_chains[m_currentChain] =
        spToolChain(new ToolChain(_config.genesis(), _config.state(), _config.sealEngine(), _config.fork(), _toolPath));
    m_pendingBlock = spEthereumBlock(new EthereumBlock(m_chains[m_currentChain].getContent().lastBlock().header()));
}


}  // namespace toolimpl
