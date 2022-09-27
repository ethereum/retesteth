#pragma once
#include "../../basetypes.h"
#include "BlockHeader.h"
#include <libdevcore/RLP.h>
#include <libdataobj/DataObject.h>

namespace test
{
namespace teststruct
{
struct BlockHeader1559 : BlockHeader
{
    BlockHeader1559(DataObject const&);
    BlockHeader1559(dev::RLP const&);

    spDataObject asDataObject() const override;
    dev::RLPStream const asRLPStream() const override;
    BlockType type() const override { return BlockType::BlockHeader1559; }

    // Unique fields
    VALUE const& baseFee() const { return m_baseFee; }
    void setBaseFee(VALUE const& _baseFee) { m_baseFee = spVALUE(_baseFee.copy()); }

    // Static
    static BlockHeader1559 const& castFrom(spBlockHeader const& _from);
    static BlockHeader1559& castFrom(BlockHeader& _from);

protected:
    BlockHeader1559(){};
    void fromData(DataObject const&) override;

    // Ethereum eip1559 blockheader fields
    spVALUE m_baseFee;
};

typedef GCP_SPointer<BlockHeader1559> spBlockHeader1559;


}  // namespace teststruct
}  // namespace test
