#pragma once
#include <libdataobj/DataObject.h>
#include <retesteth/testStructures/basetypes.h>

namespace test::teststruct
{

struct Withdrawals : GCP_SPointerBase
{
    Withdrawals(dataobject::spDataObjectMove);
    spDataObject asDataObject() const;
protected:
    struct WithdrawalRecord
    {
        WithdrawalRecord(DataObject const&);
        spDataObject asDataObject() const;
        spVALUE index;
        spVALUE validatorIndex;
        spFH20 address;
        spVALUE amount;
    };
private:
    std::vector<WithdrawalRecord> m_records;
};

typedef dataobject::GCP_SPointer<Withdrawals> spWithdrawals;

}  // namespace teststruct
