#pragma once
#include "../../basetypes.h"
#include "BlockHeader.h"
#include <libdevcore/RLP.h>
#include <retesteth/dataObject/DataObject.h>
#include <retesteth/dataObject/SPointer.h>

using namespace dataobject;
using namespace test::teststruct;

namespace test
{
namespace teststruct
{
struct BlockHeader1559 : BlockHeader
{
    BlockHeader1559(DataObject const&);
    BlockHeader1559(dev::RLP const&);
    DataObject const asDataObject() const override;
    dev::RLPStream const asRLPStream() const override;

    // bool operator==(BlockHeader1559 const& _rhs) const;
    // bool operator!=(BlockHeader1559 const& _rhs) const { return !(*this == _rhs); };

    VALUE const& gasLimit() const override;
    VALUE const& gasTarget() const { return m_gasTarget.getCContent(); }
    VALUE const& baseFeePerGas() const { return m_baseFeePerGas.getCContent(); }

    BlockType type() const override { return BlockType::BlockHeader1559; }

protected:
    BlockHeader1559(){};
    void fromData(DataObject const&) override;

    spVALUE m_gasTarget;
    spVALUE m_baseFeePerGas;
};

typedef GCP_SPointer<BlockHeader1559> spBlockHeader1559;


}  // namespace teststruct
}  // namespace test
