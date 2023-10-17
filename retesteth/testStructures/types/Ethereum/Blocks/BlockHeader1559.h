#pragma once
#include <retesteth/testStructures/basetypes.h>
#include "BlockHeaderLegacy.h"
#include <libdevcore/RLP.h>
#include <libdataobj/DataObject.h>

namespace test
{
namespace teststruct
{
struct BlockHeader1559 : BlockHeaderLegacy
{
    BlockHeader1559(DataObject const& _data) { fromData(_data); };
    BlockHeader1559(dev::RLP const&);

    virtual spDataObject asDataObject() const override;
    virtual dev::RLPStream const asRLPStream() const override;
    virtual BlockType type() const override { return BlockType::BlockHeader1559; }

    // Unique fields
    VALUE const& baseFee() const { return m_baseFee; }
    void setBaseFee(VALUE const& _baseFee) { m_baseFee = spVALUE(_baseFee.copy()); }

    // Static
    static BlockHeader1559 const& castFrom(spBlockHeader const& _from);
    static BlockHeader1559& castFrom(BlockHeader& _from);

protected:
    BlockHeader1559(){};
    virtual void checkDataScheme(DataObject const&) const override;
    virtual void _fromData(DataObject const&) override;
    virtual size_t _fromRLP(dev::RLP const&) override;
    virtual size_t _rlpHeaderSize() const override { return 16; }

    // Ethereum eip1559 blockheader fields
    spVALUE m_baseFee;
};

typedef GCP_SPointer<BlockHeader1559> spBlockHeader1559;


}  // namespace teststruct
}  // namespace test
