#pragma once
#include "TransactionBaseFee.h"

namespace test::teststruct
{
struct TransactionBlob : TransactionBaseFee
{
    TransactionBlob(DataObject const& _data) : TransactionBaseFee() { fromDataObject(_data); }
    TransactionBlob(BYTES const&);
    TransactionBlob(dev::RLP const&);

    spDataObject const asDataObject(ExportOrder _order = ExportOrder::Default) const override;
    TransactionType type() const override { return TransactionType::BLOB; }

private:
    virtual void fromRLP(dev::RLP const&) override;
    virtual dev::h256 buildVRSHash() const override;
    virtual void streamHeader(dev::RLPStream& _stream) const override;
    virtual void rebuildRLP() override;

    virtual void checkDataScheme(DataObject const&) const override;
    virtual void _fromData(DataObject const&) override;
    virtual size_t _rlpHeaderSize() const override { return 14; }

    spVALUE m_maxFeePerDataGas;
    std::vector<FH32> m_blobVersionedHashes;
};

typedef GCP_SPointer<TransactionBlob> spTransactionBlob;

}  // namespace teststruct
