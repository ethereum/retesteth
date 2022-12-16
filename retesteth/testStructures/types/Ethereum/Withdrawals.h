#pragma once
#include <libdataobj/DataObject.h>
#include <retesteth/testStructures/basetypes.h>
#include <libdevcore/RLP.h>

namespace test::teststruct
{

struct Withdrawal : GCP_SPointerBase
{
    Withdrawal(DataObject const&);
    Withdrawal(dev::RLP const&);
    spDataObject asDataObject(ExportOrder _order = ExportOrder::Default) const;
    spVALUE index;
    spVALUE validatorIndex;
    spFH20 address;
    spVALUE amount;
    const dev::RLPStream asRLPStream() const;
};

typedef dataobject::GCP_SPointer<Withdrawal> spWithdrawal;

}  // namespace teststruct
