#pragma once
#include "Transaction.h"
using namespace dataobject;
using namespace test::teststruct;

namespace test
{
namespace teststruct
{
struct TransactionAccessList : Transaction
{
    TransactionAccessList(DataObject const&, string const& _dataRawPreview = string(), string const& _dataLabel = string());
    TransactionAccessList(BYTES const&);
    TransactionAccessList(dev::RLP const&);

    DataObject const asDataObject(ExportOrder _order = ExportOrder::Default) const override;
    TransactionType type() const override { return TransactionType::ACCESSLIST; }

protected:
    TransactionAccessList() {}
    void fromRLP(dev::RLP const&) override;

    void buildVRS(VALUE const& _secret) override;
    void streamHeader(dev::RLPStream& _stream) const override;

    void rebuildRLP() override;
    spAccessList m_accessList;
};

typedef GCP_SPointer<TransactionAccessList> spTransactionAccessList;

}  // namespace teststruct
}  // namespace test
