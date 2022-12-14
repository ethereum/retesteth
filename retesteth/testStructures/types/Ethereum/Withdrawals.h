#pragma once
#include <libdataobj/DataObject.h>
#include <retesteth/testStructures/basetypes.h>

namespace test::teststruct
{

struct Withdrawal : GCP_SPointerBase
{
    Withdrawal(DataObject const&);
    spDataObject asDataObject() const;
    spVALUE index;
    spVALUE validatorIndex;
    spFH20 address;
    spVALUE amount;
};

typedef dataobject::GCP_SPointer<Withdrawal> spWithdrawal;

}  // namespace teststruct
