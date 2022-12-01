#pragma once
#include "ToolChain.h"
#include <retesteth/testStructures/types/Ethereum/BlockHeader.h>

namespace toolimpl
{

// Tool Logic
struct ChainOperationParams
{
    static ChainOperationParams defaultParams(ToolParams const& _params);
    dev::bigint minimumDifficulty;
    dev::bigint difficultyBoundDivisor;
    dev::bigint durationLimit;
    dev::bigint homesteadForkBlock;
    dev::bigint byzantiumForkBlock;
    dev::bigint muirGlacierForkBlock;
    dev::bigint constantinopleForkBlock;
    dev::bigint londonForkBlock;
};
std::tuple<VALUE, FORK> prepareReward(SealEngine _engine, FORK const& _fork, VALUE const& _blockNumber, VALUE const& _currentTD);
VALUE calculateGasLimit(VALUE const& _parentGasLimit, VALUE const& _parentGasUsed);
VALUE calculateEthashDifficulty(
    ChainOperationParams const& _chainParams, BlockHeader const& _bi, BlockHeader const& _parent);
VALUE calculateEIP1559BaseFee(ChainOperationParams const& _chainParams, spBlockHeader const& _bi, spBlockHeader const& _parent);
State restoreFullState(DataObject& _toolState);

}  // namespace toolimpl
