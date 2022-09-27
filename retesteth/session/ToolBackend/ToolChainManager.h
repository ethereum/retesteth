#pragma once
#include "ToolChain.h"
#include <retesteth/testStructures/types/RPC/EthGetBlockBy.h>
#include <retesteth/testStructures/types/RPC/SetChainParamsArgs.h>
#include <retesteth/testStructures/types/RPC/TestRawTranasction.h>
#include <boost/filesystem/path.hpp>

namespace toolimpl
{
namespace fs = boost::filesystem;

// Manage test blockchains for reorg
// Manage pending block for eth_sendRawTransaction
// Imports raw blocks (RLP) into apropriate chain, reorg if needed
class ToolChainManager : public GCP_SPointerBase
{
public:
    ToolChainManager(spSetChainParamsArgs const& _config, fs::path const& _toolPath, fs::path const& _tmpDir);
    void addPendingTransaction(spTransaction const& _tr) { m_pendingBlock.getContent().addTransaction(_tr); }

    ToolChain const& currentChain() const
    {
        assert(m_chains.count(m_currentChain));
        return m_chains.at(m_currentChain);
    }
    spDataObject const mineBlocks(size_t _number, ToolChain::Mining _req = ToolChain::Mining::AllowFailTransactions);
    FH32 importRawBlock(BYTES const& _rlp);

    EthereumBlockState const& lastBlock() const { return currentChain().lastBlock(); }
    EthereumBlockState const& blockByNumber(VALUE const& _number) const;
    EthereumBlockState const& blockByHash(FH32 const& _hash) const;
    void rewindToBlock(VALUE const& _number);
    void modifyTimestamp(VALUE const& _time);

    // Transaction tests
    static TestRawTransaction test_rawTransaction(
        BYTES const& _rlp, FORK const& _fork, fs::path const& _toolPath, fs::path const& _tmpDir);

    // Difficulty tests
    static VALUE test_calculateDifficulty(FORK const& _fork, VALUE const& _blockNumber, VALUE const& _parentTimestamp,
        VALUE const& _parentDifficulty, VALUE const& _currentTimestamp, VALUE const& _uncleNumber,
        fs::path const& _toolPath, fs::path const& _tmpDir);


private:
    ToolChainManager() {}
    ToolChain& currentChainUnsafe()
    {
        assert(m_chains.count(m_currentChain));
        return m_chains.at(m_currentChain).getContent();
    }
    EthGetBlockBy internalConstructResponseBlock(EthereumBlock const& _block) const;
    void reorganizeChainForParent(FH32 const& _parentHash);
    void reorganizeChainForTotalDifficulty();
    void reorganizePendingBlock();

    std::map<size_t, spToolChain> m_chains;
    size_t m_currentChain;
    size_t m_maxChains;
    spEthereumBlockState m_pendingBlock;

    fs::path m_tmpDir;
    fs::path m_toolPath;

private:
    void init1559PendingBlock(EthereumBlockState const&);
    void initMergePendingBlock(EthereumBlockState const&);
    bool isTerminalPoWBlock();
};

}  // namespace toolimpl
