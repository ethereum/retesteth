#pragma once
#include "TransactionAccessList.h"
using namespace dataobject;
using namespace test::teststruct;

namespace test
{
namespace teststruct
{
struct TransactionBaseFee : TransactionAccessList
{
    TransactionBaseFee(DataObject const&, string const& _dataRawPreview = string(), string const& _dataLabel = string());
    TransactionBaseFee(BYTES const&);
    TransactionBaseFee(dev::RLP const&);

    DataObject const asDataObject(ExportOrder _order = ExportOrder::Default) const override;
    TransactionType type() const override { return TransactionType::BASEFEE; }

    VALUE const& feeCap() const { return m_feeCap.getCContent(); }
    VALUE const& tip() const { return m_tip.getCContent(); }

private:
    void fromRLP(dev::RLP const&) override;
    void fromDataObject(DataObject const&) override;

    void buildVRS(VALUE const& _secret) override;
    void streamHeader(dev::RLPStream& _stream) const override;

    void rebuildRLP() override;
    spVALUE m_feeCap;
    spVALUE m_tip;
};

typedef GCP_SPointer<TransactionBaseFee> spTransactionBaseFee;

}  // namespace teststruct
}  // namespace test
