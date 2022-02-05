#pragma once
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
    BlockMining(fs::path const& _toolPath, fs::path const& _tmpDir, EthereumBlockState const& _currentBlock,
        EthereumBlockState const& _parentBlock, SealEngine _engine, spFORK const& _fork,
        std::vector<EthereumBlockState> const& _blockHistory)
      : m_toolPathRef(_toolPath),
        m_engine(_engine),
        m_forkRef(_fork),
        m_tmpDirRef(_tmpDir),
        m_currentBlockRef(_currentBlock),
        m_parentBlockRef(_parentBlock),
        m_blockHistoryRef(_blockHistory)
    {}
    ~BlockMining();

    void prepareEnvFile();
    void prepareAllocFile();
    void prepareTxnFile();
    void executeTransition();
    ToolResponse readResult();

private:
    fs::path const& m_toolPathRef;
    SealEngine m_engine;
    spFORK const& m_forkRef;
    fs::path const& m_tmpDirRef;
    EthereumBlockState const& m_currentBlockRef;
    EthereumBlockState const& m_parentBlockRef;
    std::vector<EthereumBlockState> const& m_blockHistoryRef;

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
