#pragma once
#include "TransactionLegacy.h"
#include <retesteth/testStructures/types/StateTests/Base/AccessList.h>

namespace test
{
namespace teststruct
{

struct TransactionAccessList : TransactionLegacy
{
    TransactionAccessList(DataObject const& _data) : TransactionLegacy() { fromDataObject(_data); }
    TransactionAccessList(BYTES const&);
    TransactionAccessList(dev::RLP const&);

    spDataObject const asDataObject(ExportOrder _order = ExportOrder::Default) const override;
    TransactionType type() const override { return TransactionType::ACCESSLIST; }

protected:
    TransactionAccessList() {}

    // Override protected interface
    virtual void fromRLP(dev::RLP const&) override;
    virtual dev::h256 buildVRSHash() const override;
    virtual void buildVRS() override;
    virtual void streamHeader(dev::RLPStream& _stream) const override;
    virtual void rebuildRLP() override;

    virtual void checkDataScheme(DataObject const&) const override;
    virtual void _fromData(DataObject const&) override;
    virtual size_t _rlpHeaderSize() const override { return 11; }

    // Transaction access list specific field
    spAccessList m_accessList;
};

typedef GCP_SPointer<TransactionAccessList> spTransactionAccessList;

}  // namespace teststruct
}  // namespace test
