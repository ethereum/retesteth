#pragma once
#include "ToolChain.h"
#include "Verification.h"
#include <libdevcore/Common.h>
#include <retesteth/testStructures/types/Ethereum/BlockHeader.h>

using namespace dev;
namespace toolimpl
{

// Tool Logic
struct ChainOperationParams
{
    static ChainOperationParams defaultParams(ToolParams const& _params);
    bigint minimumDifficulty;
    bigint difficultyBoundDivisor;
    bigint durationLimit;
    bigint homesteadForkBlock;
    bigint byzantiumForkBlock;
    bigint muirGlacierForkBlock;
    bigint constantinopleForkBlock;
    bigint londonForkBlock;
};
std::tuple<VALUE, FORK> prepareReward(SealEngine _engine, FORK const& _fork, VALUE const& _blockNumber = VALUE(0));
VALUE calculateGasLimit(VALUE const& _parentGasLimit, VALUE const& _parentGasUsed);
VALUE calculateEthashDifficulty(
    ChainOperationParams const& _chainParams, spBlockHeader const& _bi, spBlockHeader const& _parent);
VALUE calculateEIP1559BaseFee(ChainOperationParams const& _chainParams, spBlockHeader const& _bi, spBlockHeader const& _parent);
State restoreFullState(DataObject const& _toolState);

}  // namespace toolimpl
