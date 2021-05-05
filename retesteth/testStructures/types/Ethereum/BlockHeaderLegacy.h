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
// Ethereum blockheader description
struct BlockHeaderLegacy : BlockHeader
{
    BlockHeaderLegacy(DataObject const&);
    BlockHeaderLegacy(dev::RLP const&);
    virtual ~BlockHeaderLegacy(){/* all smart pointers */};

    virtual DataObject const asDataObject() const override;
    virtual dev::RLPStream const asRLPStream() const override;
    virtual BlockType type() const override { return BlockType::BlockHeaderLegacy; }

    // Unique fields
    VALUE const& gasLimit() const { return m_gasLimit.getCContent(); }
    void setGasLimit(VALUE const& _gasLimit) { m_gasLimit = spVALUE(new VALUE(_gasLimit)); }

    // Static
    static BlockHeaderLegacy const& castFrom(spBlockHeader const& _from);

protected:
    BlockHeaderLegacy() {}
    virtual void fromData(DataObject const&) override;

    // Calssic ethereum blockheader fields
    spVALUE m_gasLimit;
};

typedef GCP_SPointer<BlockHeaderLegacy> spBlockHeaderLegacy;


}  // namespace teststruct
}  // namespace test
