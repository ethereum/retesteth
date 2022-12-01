#pragma once
#include "../../basetypes.h"
#include "BlockHeader1559.h"
#include <libdevcore/RLP.h>
#include <libdataobj/DataObject.h>

namespace test
{
namespace teststruct
{
struct BlockHeaderMerge : BlockHeader1559
{
    BlockHeaderMerge(DataObject const& _in) : BlockHeader1559(_in) {}
    BlockHeaderMerge(dev::RLP const& _in) : BlockHeader1559(_in) {}

    BlockType type() const override { return BlockType::BlockHeaderMerge; }

    static BlockHeaderMerge const& castFrom(spBlockHeader const& _from);
    static BlockHeaderMerge& castFrom(BlockHeader& _from);

protected:
    BlockHeaderMerge(){};
};

typedef GCP_SPointer<BlockHeaderMerge> spBlockHeaderMerge;


}  // namespace teststruct
}  // namespace test
