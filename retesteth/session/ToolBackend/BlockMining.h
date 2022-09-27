#pragma once
#include "ToolChain.h"
#include <testStructures/types/RPC/ToolResponse.h>
#include <boost/filesystem/path.hpp>

namespace toolimpl
{
namespace fs = boost::filesystem;
using namespace test::teststruct;

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
    std::string m_allocPathContent;
    fs::path m_envPath;
    std::string m_envPathContent;
    fs::path m_txsPath;
    std::string m_txsPathContent;
    fs::path m_outPath;
    fs::path m_outAllocPath;
    void traceTransactions(ToolResponse& _toolResponse);
};
}  // namespace toolimpl
