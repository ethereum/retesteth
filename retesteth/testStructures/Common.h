#pragma once
#include <retesteth/dataObject/DataObject.h>

// Base Types
#include "basetypes/BYTES.h"
#include "basetypes/FH20.h"
#include "basetypes/FH32.h"
#include "basetypes/VALUE.h"

// Test Types
#include "types/Account.h"
#include "types/State.h"
#include "types/StateIncomplete.h"
#include "types/Storage.h"

// StateTest Types
#include "types/StateTests/GeneralStateTestsFiller.h"

using namespace dataobject;
using namespace test::teststruct;

namespace test
{
namespace teststruct
{
// CHeck that DataObject is of type Object
void checkDataObject(DataObject const& _input);

// Prepare chain params rpc request
string prepareChainParams();


}  // namespace teststruct
}  // namespace test
