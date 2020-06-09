#pragma once
#include <testStructures/types/RPC/ToolResponse.h>
#include <testStructures/types/ethereum.h>
#include <boost/filesystem.hpp>
#include <vector>
namespace fs = boost::filesystem;

namespace toolimpl
{
// Manage test blockchains
class ToolChain : public GCP_SPointerBase
{
public:
    ToolChain(EthereumBlock const& _genesis, State const& _genesisState, SealEngine _sealEngine, FORK const& _fork,
        fs::path const& _toolPath);

    // Execute t8ntool cmd with input _block information, and get the output block information
    // Information includes header, transactions, state
    ToolResponse mineBlock(EthereumBlock const& _block, State const& _state);
    EthereumBlock const& lastBlock() const
    {
        assert(m_blocks.size() > 0);
        return m_blocks.at(m_blocks.size() - 1);
    }

private:
    ToolChain(){};
    std::vector<EthereumBlock> m_blocks;
    spState m_genesisState;
    SealEngine m_engine;
    spFORK m_fork;
    fs::path m_toolPath;
};

typedef GCP_SPointer<ToolChain> spToolChain;

}  // namespace toolimpl
