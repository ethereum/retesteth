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
// Indicates that Transaction is read from StateTest filler
// Constructor conver all fields to hex and compile src data code into bytes
struct StateTestFillerTransaction : StateTestTransactionBase
{
    StateTestFillerTransaction(spDataObjectMove);
    spFH20 const& getSender() const { return m_publicKey; }
};

typedef GCP_SPointer<StateTestFillerTransaction> spStateTestFillerTransaction;

}  // namespace teststruct
}  // namespace test
