#pragma once
#include "Transaction.h"
#include <retesteth/testStructures/types/StateTests/Base/AccessList.h>

namespace test
{
namespace teststruct
{
struct TransactionBaseFee : Transaction
{
    TransactionBaseFee(DataObject const& _data) : Transaction() { fromDataObject(_data); }
    TransactionBaseFee(BYTES const&);
    TransactionBaseFee(dev::RLP const&);

    spDataObject const asDataObject(ExportOrder _order = ExportOrder::Default) const override;
    TransactionType type() const override { return TransactionType::BASEFEE; }

private:
    virtual void fromRLP(dev::RLP const&) override;
    virtual dev::h256 buildVRSHash() const override;
    virtual void buildVRS() override;
    virtual void streamHeader(dev::RLPStream& _stream) const override;
    virtual void rebuildRLP() override;

    virtual void checkDataScheme(DataObject const&) const override;
    virtual void _fromData(DataObject const&) override;
    virtual size_t _rlpHeaderSize() const override { return 12; }

    spAccessList m_accessList;
    spVALUE m_maxFeePerGas;
    spVALUE m_maxPriorityFeePerGas;
};

typedef GCP_SPointer<TransactionBaseFee> spTransactionBaseFee;

}  // namespace teststruct
}  // namespace test
