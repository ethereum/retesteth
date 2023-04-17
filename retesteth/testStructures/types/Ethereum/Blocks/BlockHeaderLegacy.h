#pragma once
#include <retesteth/testStructures/basetypes.h>
#include "BlockHeader.h"
#include <libdevcore/RLP.h>
#include <libdataobj/DataObject.h>

namespace test
{
namespace teststruct
{
// Ethereum blockheader description
struct BlockHeaderLegacy : BlockHeader
{
    BlockHeaderLegacy(DataObject const& _data) { fromData(_data); };
    BlockHeaderLegacy(dev::RLP const&);
    virtual ~BlockHeaderLegacy(){/* all smart pointers */};

    virtual spDataObject asDataObject() const override;
    virtual dev::RLPStream const asRLPStream() const override;
    virtual BlockType type() const override { return BlockType::BlockHeaderLegacy; }

    // Static
    static BlockHeaderLegacy const& castFrom(spBlockHeader const& _from);

protected:
    BlockHeaderLegacy() {}
    virtual void fromData(DataObject const&) override;
    virtual void checkDataScheme(DataObject const&) override;
    virtual void _fromData(DataObject const&) override;
    virtual size_t _fromRLP(dev::RLP const&) override;
    virtual size_t _rlpHeaderSize() const override { return 15; }
};

typedef GCP_SPointer<BlockHeaderLegacy> spBlockHeaderLegacy;


}  // namespace teststruct
}  // namespace test
