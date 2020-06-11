#pragma once
#include "../../../basetypes.h"
#include "../../../types/ethereum.h"
#include "../Base/StateTestTransactionBase.h"
#include <retesteth/dataObject/DataObject.h>
#include <retesteth/dataObject/SPointer.h>
using namespace dataobject;
using namespace test::teststruct;

namespace test
{
namespace teststruct
{
// Sructure to store JSON data from test
// Indicates that Transaction is read from StateTest filler
// Constructor conver all fields to hex and compile src data code into bytes
struct StateTestFillerTransaction : StateTestTransactionBase
{
    StateTestFillerTransaction(DataObject const&);
};

}  // namespace teststruct
}  // namespace test
