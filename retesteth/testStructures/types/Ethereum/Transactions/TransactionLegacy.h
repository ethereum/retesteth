#pragma once
#include <retesteth/testStructures/basetypes.h>
#include "Transaction.h"
#include <libdevcore/RLP.h>
#include <libdataobj/DataObject.h>

namespace test::teststruct
{

struct TransactionLegacy : Transaction
{
    TransactionLegacy(DataObject const& _data) : Transaction() { fromDataObject(_data); }
    TransactionLegacy(BYTES const&);
    TransactionLegacy(dev::RLP const&);

    // Transaction legacy fields
    VALUE const& gasPrice() const { return m_gasPrice; }

    virtual TransactionType type() const override { return TransactionType::LEGACY; }
    virtual spDataObject const asDataObject(ExportOrder _order = ExportOrder::Default) const override;

protected:
    TransactionLegacy() {}

    // Potected transaction interface
    virtual void fromRLP(dev::RLP const&) override;
    virtual dev::h256 buildVRSHash() const override;
    virtual void buildVRS() override;
    virtual void streamHeader(dev::RLPStream& _stream) const override;
    virtual void rebuildRLP() override;

    virtual void checkDataScheme(DataObject const&) const override;
    virtual void _fromData(DataObject const&) override;
    virtual size_t _rlpHeaderSize() const override { return 9; }

    spVALUE m_gasPrice;
};

typedef GCP_SPointer<TransactionLegacy> spTransactionLegacy;

}  // namespace teststruct
