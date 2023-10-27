#pragma once
#include "ToolChain.h"
#include <retesteth/testStructures/types/RPC/EthGetBlockBy.h>
#include <retesteth/testStructures/types/RPC/SetChainParamsArgs.h>
#include <retesteth/testStructures/types/RPC/TestRawTranasction.h>
#include <boost/filesystem/path.hpp>

namespace toolimpl
{

// Manage test blockchains for reorg
// Manage pending block for eth_sendRawTransaction
// Imports raw blocks (RLP) into apropriate chain, reorg if needed
class ToolChainManager : public GCP_SPointerBase
{
public:
    ToolChainManager(spSetChainParamsArgs const& _config, boost::filesystem::path const& _toolPath, boost::filesystem::path const& _tmpDir, ToolChainGenesis _genesisPolicy = ToolChainGenesis::CALCULATE);
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
    void registerWithdrawal(BYTES const& _wt);

    // Transaction tests
    static TestRawTransaction test_rawTransaction(
        BYTES const& _rlp, FORK const& _fork, boost::filesystem::path const& _toolPath, boost::filesystem::path const& _tmpDir);

    // EOF tests
    static std::string test_rawEOFCode(
        BYTES const& _code, FORK const& _fork, boost::filesystem::path const& _toolPath, boost::filesystem::path const& _tmpDir);

    // Difficulty tests
    static VALUE test_calculateDifficulty(FORK const& _fork, VALUE const& _blockNumber, VALUE const& _parentTimestamp,
        VALUE const& _parentDifficulty, VALUE const& _currentTimestamp, VALUE const& _uncleNumber,
        boost::filesystem::path const& _toolPath, boost::filesystem::path const& _tmpDir);


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
    bool isMergeChain() const;


    std::map<size_t, spToolChain> m_chains;
    size_t m_currentChain;
    size_t m_maxChains;
    spEthereumBlockState m_pendingBlock;

    boost::filesystem::path m_tmpDir;
    boost::filesystem::path m_toolPath;

private:
    void transitionPendingBlock(EthereumBlockState const&);
    void init1559PendingBlock(EthereumBlockState const&);
    void initMergePendingBlock(EthereumBlockState const&);
    void initShanghaiPendingBlock(EthereumBlockState const&);
    void initCancunPendingBlock(EthereumBlockState const&);
    bool isTerminalPoWBlock();

private:
    spBlockHeader _irb_verifyAndSetHeader(dev::RLP const&);
    void _irb_verifyAndSetTransactions(dev::RLP const&);
    void _irb_verifyAndSetUncles(dev::RLP const&, spBlockHeader const&);
    void _irb_verifyAndSetWithdrawals(dev::RLP const&, spBlockHeader const&);
    FH32 _irb_compareT8NBlockToRawRLP(spBlockHeader const&);
};

}  // namespace toolimpl
