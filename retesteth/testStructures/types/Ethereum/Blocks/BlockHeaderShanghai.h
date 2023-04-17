#pragma once
#include <retesteth/testStructures/basetypes.h>
#include "BlockHeaderMerge.h"
#include <libdevcore/RLP.h>
#include <libdataobj/DataObject.h>

namespace test::teststruct
{
struct BlockHeaderShanghai : BlockHeaderMerge
{
    BlockHeaderShanghai(DataObject const& _in);
    BlockHeaderShanghai(dev::RLP const& _in);

    spDataObject asDataObject() const override;
    dev::RLPStream const asRLPStream() const override;
    BlockType type() const override { return BlockType::BlockHeaderShanghai; }

    FH32 const& withdrawalsRoot() const { return m_withdrawalsRoot; }
    void setWithdrawalsRoot(FH32 const& _wRoot) { m_withdrawalsRoot = spFH32(_wRoot.copy()); }

    static BlockHeaderShanghai const& castFrom(spBlockHeader const& _from);
    static BlockHeaderShanghai& castFrom(BlockHeader& _from);

protected:
    void fromData(DataObject const&) override;
    void checkDataScheme(DataObject const&) override;

    spFH32 m_withdrawalsRoot;
    BlockHeaderShanghai(){};
};

typedef GCP_SPointer<BlockHeaderShanghai> spBlockHeaderShanghai;


}  // namespace teststruct
