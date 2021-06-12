#pragma once
#include <testStructures/types/RPC/SetChainParamsArgs.h>
#include <testStructures/types/RPC/ToolResponse.h>
#include <testStructures/types/ethereum.h>
#include <boost/filesystem.hpp>
#include <vector>
namespace fs = boost::filesystem;

namespace toolimpl
{
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

// Manage test blockchains
class ToolChain : public GCP_SPointerBase
{
public:
    ToolChain(EthereumBlockState const& _genesis, SetChainParamsArgs const& _params, fs::path const& _toolPath,
        fs::path const& _tmpDir);

    EthereumBlockState const& lastBlock() const
    {
        assert(m_blocks.size() > 0);
        return m_blocks.at(m_blocks.size() - 1);
    }

    std::vector<EthereumBlockState> const& blocks() const { return m_blocks; }
    SealEngine engine() const { return m_engine; }
    FORK const& fork() const { return m_fork; }
    fs::path const& toolPath() const { return m_toolPath; }
    SetChainParamsArgs const& params() const { return m_initialParams; }
    ToolParams const& toolParams() const { return m_toolParams; }

    enum class Mining
    {
        RequireValid,
        AllowFailTransactions
    };
    DataObject const mineBlock(EthereumBlockState const& _pendingBlock, Mining _req = Mining::AllowFailTransactions);
    void rewindToBlock(size_t _number);

    // Used for chain reorg
    void insertBlock(EthereumBlockState const& _block) { m_blocks.push_back(_block); }
    fs::path const& tmpDir() const { return m_tmpDir; }

private:
    ToolChain(){};
    // Execute t8ntool cmd with input _block information, and get the output block information
    // Information includes header, transactions, state
    ToolResponse mineBlockOnTool(EthereumBlockState const& _block, SealEngine _engine = SealEngine::NoReward);

    GCP_SPointer<ToolParams> m_toolParams;
    GCP_SPointer<SetChainParamsArgs> m_initialParams;
    std::vector<EthereumBlockState> m_blocks;
    SealEngine m_engine;
    spFORK m_fork;
    fs::path m_toolPath;
    fs::path m_tmpDir;
};

typedef GCP_SPointer<ToolChain> spToolChain;
}  // namespace toolimpl
