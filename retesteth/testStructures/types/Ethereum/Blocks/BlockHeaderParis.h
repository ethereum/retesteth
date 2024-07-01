#pragma once
#include <retesteth/testStructures/basetypes.h>
#include "BlockHeader1559.h"
#include <libdevcore/RLP.h>
#include <libdataobj/DataObject.h>

namespace test
{
namespace teststruct
{
struct BlockHeaderParis : BlockHeader1559
{
    BlockHeaderParis(DataObject const& _in) : BlockHeader1559(_in) {}
    BlockHeaderParis(dev::RLP const& _in) : BlockHeader1559(_in) {}

    virtual BlockType type() const override { return BlockType::BlockHeaderParis; }

    static BlockHeaderParis const& castFrom(spBlockHeader const& _from);
    static BlockHeaderParis& castFrom(BlockHeader& _from);

protected:
    BlockHeaderParis(){};
};

typedef GCP_SPointer<BlockHeaderParis> spBlockHeaderParis;


}  // namespace teststruct
}  // namespace test
