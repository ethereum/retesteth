#pragma once
#include "ToolChain.h"
#include <testStructures/types/RPC/ToolResponse.h>
#include <testStructures/types/ethereum.h>
#include <boost/filesystem/path.hpp>
namespace fs = boost::filesystem;

namespace toolimpl
{
// t8n tool generation of new block
class BlockMining
{
public:
    BlockMining(ToolChain const& _toolChain, EthereumBlockState const& _currentBlock, EthereumBlockState const& _parentBlock,
        SealEngine _engine)
      : m_chainRef(_toolChain), m_currentBlockRef(_currentBlock), m_parentBlockRef(_parentBlock), m_engine(_engine)
    {}
    ~BlockMining();

    void prepareEnvFile();
    void prepareAllocFile();
    void prepareTxnFile();
    void executeTransition();
    ToolResponse readResult();

private:
    ToolChain const& m_chainRef;
    EthereumBlockState const& m_currentBlockRef;
    EthereumBlockState const& m_parentBlockRef;
    SealEngine m_engine;

private:
    fs::path m_allocPath;
    string m_allocPathContent;
    fs::path m_envPath;
    string m_envPathContent;
    fs::path m_txsPath;
    string m_txsPathContent;
    fs::path m_outPath;
    fs::path m_outAllocPath;
    void traceTransactions(ToolResponse& _toolResponse);
};
}  // namespace toolimpl
