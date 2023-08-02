#pragma once
#include <testStructures/types/Ethereum/EthereumBlock.h>
#include <testStructures/types/RPC/SetChainParamsArgs.h>
#include <testStructures/types/RPC/ToolResponse.h>
#include <boost/filesystem/path.hpp>
#include <vector>
namespace toolimpl
{
using namespace test::teststruct;

// Parse SetChainParams::params section
struct ToolParams : GCP_SPointerBase
{
    ToolParams(DataObject const&);
    VALUE const& homesteadForkBlock() const { return m_homesteadForkBlock; }
    VALUE const& byzantiumForkBlock() const { return m_byzantiumForkBlock; }
    VALUE const& constantinopleForkBlock() const { return m_constantinopleForkBlock; }
    VALUE const& muirGlacierForkBlock() const { return m_muirGlacierForkBlock; }
    VALUE const& londonForkBlock() const { return m_londonForkBlock; }

private:
    ToolParams();
    spVALUE m_homesteadForkBlock;
    spVALUE m_byzantiumForkBlock;
    spVALUE m_constantinopleForkBlock;
    spVALUE m_muirGlacierForkBlock;
    spVALUE m_londonForkBlock;
};

enum class ToolChainGenesis
{
    CALCULATE,
    NOTCALCULATE
};

// Manage test blockchains
class ToolChain : public GCP_SPointerBase
{
public:
    ToolChain(EthereumBlockState const& _genesis, spSetChainParamsArgs const& _params, boost::filesystem::path const& _toolPath,
        boost::filesystem::path const& _tmpDir, ToolChainGenesis _genesisPolicy = ToolChainGenesis::CALCULATE);

    // Calculate difficulty from _blockA to _blockB constructor
    ToolChain(EthereumBlockState const& _blockA, EthereumBlockState const& _blockB, FORK const& _fork,
        boost::filesystem::path const& _toolPath, boost::filesystem::path const& _tmpDir);

    EthereumBlockState const& lastBlock() const
    {
        assert(m_blocks.size() > 0);
        return m_blocks.at(m_blocks.size() - 1);
    }

    std::vector<EthereumBlockState> const& blocks() const { return m_blocks; }
    SealEngine engine() const { return m_engine; }
    FORK const& fork() const { return m_fork; }
    boost::filesystem::path const& toolPath() const { return m_toolPath; }
    spSetChainParamsArgs const& params() const { return m_initialParams; }
    ToolParams const& toolParams() const { return m_toolParams; }

    enum class Mining
    {
        RequireValid,
        AllowFailTransactions
    };
    spDataObject const mineBlock(EthereumBlockState const& _pendingBlock, EthereumBlockState const& _parentBlock, Mining _req = Mining::AllowFailTransactions);
    void rewindToBlock(size_t _number);

    // Used for chain reorg
    void insertBlock(EthereumBlockState const& _block) { m_blocks.emplace_back(_block); }
    boost::filesystem::path const& tmpDir() const { return m_tmpDir; }

private:
    ToolChain(){};
    // Execute t8ntool cmd with input _block information, and get the output block information
    // Information includes header, transactions, state
    ToolResponse mineBlockOnTool(EthereumBlockState const& _currentBlock, EthereumBlockState const& _parentBlock,
        SealEngine _engine = SealEngine::NoReward);

    GCP_SPointer<ToolParams> m_toolParams;
    const spSetChainParamsArgs m_initialParams;
    std::vector<EthereumBlockState> m_blocks;
    SealEngine m_engine;
    spFORK m_fork;
    boost::filesystem::path m_toolPath;
    boost::filesystem::path m_tmpDir;

private:
    void checkDifficultyAgainstRetesteth(VALUE const& _toolDifficulty, spBlockHeader const& _pendingHeader);
    void checkBasefeeAgainstRetesteth(VALUE const& _toolBasefee, spBlockHeader const& _pendingHeader, spBlockHeader const& _parentHeader);
    void calculateAndCheckSetBaseFee(VALUE const& _toolBaseFee, spBlockHeader& _pendingHeader, spBlockHeader const& _parentHeader);
    void setWithdrawalsRoot(FH32 const&, spBlockHeader&);
    void setExcessBlobGasAndGasUsed(ToolResponse const&, spBlockHeader&);
    void setAndCheckDifficulty(VALUE const&, spBlockHeader&);

    spDataObject coorectTransactionsByToolResponse(ToolResponse const& _res, EthereumBlockState& _pendingFixed,
        EthereumBlockState const& _pendingBlock, Mining _miningReq);
    void correctUncleHeaders(EthereumBlockState& _pendingFixed, EthereumBlockState const& _pendingBlock);
    void additionalHeaderVerification(ToolResponse const& _res, EthereumBlockState& _pendingFixed,
        EthereumBlockState const& _pendingBlock, Mining _miningReq);
    void calculateAndSetTotalDifficulty(EthereumBlockState& _pendingFixed);
};

typedef GCP_SPointer<ToolChain> spToolChain;
}  // namespace toolimpl
