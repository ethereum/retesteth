#pragma once
#include "Transaction.h"
namespace test
{
namespace teststruct
{
struct TransactionBaseFee : Transaction
{
    TransactionBaseFee(DataObject const&);
    TransactionBaseFee(BYTES const&);
    TransactionBaseFee(dev::RLP const&);

    spDataObject const asDataObject(ExportOrder _order = ExportOrder::Default) const override;
    TransactionType type() const override { return TransactionType::BASEFEE; }

private:
    void fromRLP(dev::RLP const&) override;
    void fromDataObject(DataObject const&) override;

    void buildVRS(VALUE const& _secret) override;
    void streamHeader(dev::RLPStream& _stream) const override;

    void rebuildRLP() override;
    spAccessList m_accessList;
    spVALUE m_maxFeePerGas;
    spVALUE m_maxPriorityFeePerGas;
};

typedef GCP_SPointer<TransactionBaseFee> spTransactionBaseFee;

}  // namespace teststruct
}  // namespace test
