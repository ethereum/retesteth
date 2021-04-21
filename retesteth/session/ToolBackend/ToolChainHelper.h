#pragma once
#include "ToolChain.h"
#include <libdevcore/Common.h>
#include <retesteth/testStructures/types/Ethereum/BlockHeader.h>

using namespace dev;
namespace toolimpl
{
// Blockchain logic validator
void verifyEthereumBlockHeader(spBlockHeader const& _header, ToolChain const& _chain);

// Tool Logic
struct ChainOperationParams
{
    static ChainOperationParams defaultParams(ToolParams const& _params);
    u256 minimumDifficulty;
    u256 difficultyBoundDivisor;
    u256 durationLimit;
    u256 homesteadForkBlock;
    u256 byzantiumForkBlock;
    u256 muirGlacierForkBlock;
    u256 constantinopleForkBlock;
};
std::tuple<VALUE, FORK> prepareReward(SealEngine _engine, FORK const& _fork, VALUE const& _blockNumber = VALUE(0));
VALUE calculateGasLimit(VALUE const& _parentGasLimit, VALUE const& _parentGasUsed);
u256 calculateEthashDifficulty(
    ChainOperationParams const& _chainParams, spBlockHeader const& _bi, spBlockHeader const& _parent);
State restoreFullState(DataObject const& _toolState);

}  // namespace toolimpl
