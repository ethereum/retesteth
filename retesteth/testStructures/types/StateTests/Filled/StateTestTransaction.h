#pragma once
#include "../../../basetypes.h"
#include "../../../types/ethereum.h"
#include "../Base/StateTestTransactionBase.h"
#include <libdataobj/DataObject.h>
#include <libdataobj/SPointer.h>
namespace test
{
namespace teststruct
{
// Sructure to store JSON data from test
// Indicates that Transaction is read from filled StateTest
// Constructor expect all fields hex, and don't compile data into bytecode
// Bytecode is required to be already compiled
struct StateTestTransaction : StateTestTransactionBase
{
    StateTestTransaction(DataObject const&);
};

}  // namespace teststruct
}  // namespace test
