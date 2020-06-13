#pragma once

#include "configs/SealEngine.h"
#include "types/Ethereum/State.h"
#include "types/StateTests/Base/StateTestEnvBase.h"
#include <retesteth/dataObject/DataObject.h>
#include <retesteth/testStructures/types/RPC/SetChainParamsArgs.h>
using namespace dataobject;

namespace test
{
namespace teststruct
{
// Prepare chain params rpc request
SetChainParamsArgs prepareChainParams(FORK const&, SealEngine, State const&, StateTestEnvBase const&);

}  // namespace teststruct
}  // namespace test
