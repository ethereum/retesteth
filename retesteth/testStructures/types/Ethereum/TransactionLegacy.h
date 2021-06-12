#pragma once
#include "../../basetypes.h"
#include "Transaction.h"
#include <libdevcore/RLP.h>
#include <retesteth/dataObject/DataObject.h>
#include <retesteth/testStructures/types/StateTests/Base/AccessList.h>
using namespace dataobject;
using namespace test::teststruct;

namespace test
{
namespace teststruct
{

struct TransactionLegacy : Transaction
{
    TransactionLegacy(DataObject const&);
    TransactionLegacy(BYTES const&);
    TransactionLegacy(dev::RLP const&);

    // Transaction legacy fields
    VALUE const& gasPrice() const { return m_gasPrice; }

    virtual TransactionType type() const override { return TransactionType::LEGACY; }
    virtual DataObject const asDataObject(ExportOrder _order = ExportOrder::Default) const override;

protected:
    TransactionLegacy() {}

    // Potected transaction interface
    virtual void fromRLP(dev::RLP const&) override;
    virtual void fromDataObject(DataObject const&) override;
    virtual void buildVRS(VALUE const& _secret) override;
    virtual void streamHeader(dev::RLPStream& _stream) const override;
    virtual void rebuildRLP() override;

    spVALUE m_gasPrice;
};

typedef GCP_SPointer<TransactionLegacy> spTransactionLegacy;

}  // namespace teststruct
}  // namespace test
