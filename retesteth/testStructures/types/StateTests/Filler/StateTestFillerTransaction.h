#pragma once
#include "../../../basetypes.h"
#include "../Base/StateTestTransactionBase.h"
#include <libdataobj/DataObject.h>
namespace test::teststruct
{
// Sructure to store JSON data from test
// Indicates that Transaction is read from StateTest filler
// Constructor conver all fields to hex and compile src data code into bytes
struct StateTestFillerTransaction : StateTestTransactionBase
{
    StateTestFillerTransaction(spDataObjectMove);
    spFH20 const& getSender() const { return m_publicKey; }
    spDataObject asDataObject(size_t _d, size_t _g, size_t v) const;
    spDataObject asDataObject() const { return StateTestTransactionBase::asDataObject(); }
};

typedef GCP_SPointer<StateTestFillerTransaction> spStateTestFillerTransaction;

}  // namespace teststruct
