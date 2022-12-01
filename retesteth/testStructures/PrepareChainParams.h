#pragma once

#include "configs/SealEngine.h"
#include "types/Ethereum/State.h"
#include "types/StateTests/Base/StateTestEnvBase.h"
#include <retesteth/testStructures/types/RPC/SetChainParamsArgs.h>

namespace test::teststruct
{
enum class ParamsContext
{
    StateTests,
    BlockchainTests
};

// Prepare chain params rpc request
spSetChainParamsArgs prepareChainParams(FORK const&, SealEngine, State const&, StateTestEnvBase const&,
    ParamsContext _paramsContext = ParamsContext::BlockchainTests);

}  // namespace teststruct
