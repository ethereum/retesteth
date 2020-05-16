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
#include "types/StateTests/StateTestFillerTransaction.h"

using namespace dataobject;
using namespace test::teststruct;

namespace test
{
namespace teststruct
{
// Cueck that _input contains exactly 1 subobject
void checkOnlyOneTest(DataObject const& _input);


}  // namespace teststruct
}  // namespace test
