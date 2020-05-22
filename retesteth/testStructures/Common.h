#pragma once
#include <retesteth/dataObject/DataObject.h>

// Configs
#include "configs/FORK.h"
#include "configs/SealEngine.h"
#include "configs/ClientConfig.h"

// Base Types
#include "basetypes.h"
#include "types/ethereum.h"

// StateTest Types
#include "types/StateTests/Filler/StateTestFillerEnv.h"
#include "types/StateTests/GeneralStateTest.h"
#include "types/StateTests/GeneralStateTestFiller.h"

// RPC response Types
#include "types/rpc.h"

using namespace dataobject;
using namespace test::teststruct;

namespace test
{
namespace teststruct
{
// CHeck that DataObject is of type Object
// void checkDataObject(DataObject const& _input);

// Prepare chain params rpc request
DataObject prepareChainParams(FORK const&, SealEngine, State const&, StateTestFillerEnv const&);

}  // namespace teststruct
}  // namespace test
