#pragma once
#include "TransactionLegacy.h"

namespace test
{
namespace teststruct
{

struct TransactionAccessList : TransactionLegacy
{
    TransactionAccessList(DataObject const&);
    TransactionAccessList(BYTES const&);
    TransactionAccessList(dev::RLP const&);

    spDataObject const asDataObject(ExportOrder _order = ExportOrder::Default) const override;
    TransactionType type() const override { return TransactionType::ACCESSLIST; }

protected:
    TransactionAccessList() {}

    // Override protected interface
    void fromRLP(dev::RLP const&) override;
    void fromDataObject(DataObject const&) override;
    void buildVRS(VALUE const& _secret) override;
    void streamHeader(dev::RLPStream& _stream) const override;
    void rebuildRLP() override;

    // Transaction access list specific field
    spAccessList m_accessList;
};

typedef GCP_SPointer<TransactionAccessList> spTransactionAccessList;

}  // namespace teststruct
}  // namespace test
