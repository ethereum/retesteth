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

    BYTES const getSignedRLP() const override;
    dev::RLPStream const asRLPStream() const override;
    DataObject const asDataObject(ExportOrder _order = ExportOrder::Default) const override;

    FH32 hash() const override;
    TransactionType type() const override { return TransactionType::ACCESSLIST; }

private:
    // void fromDataObject(DataObject const&) override;
    void fromRLP(dev::RLP const&) override;

    void buildVRS(VALUE const& _secret) override;
    void streamHeader(dev::RLPStream& _stream) const override;
    AccessList m_accessList;
};

typedef GCP_SPointer<TransactionAccessList> spTransactionAccessList;

}  // namespace teststruct
}  // namespace test
