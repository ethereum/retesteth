#pragma once
#include <retesteth/testStructures/basetypes.h>
#include "BlockHeaderParis.h"
#include <libdevcore/RLP.h>
#include <libdataobj/DataObject.h>

namespace test::teststruct
{
struct BlockHeaderShanghai : BlockHeaderParis
{
    BlockHeaderShanghai(DataObject const& _in) { fromData(_in); };
    BlockHeaderShanghai(dev::RLP const& _in);

    virtual spDataObject asDataObject() const override;
    virtual dev::RLPStream const asRLPStream() const override;
    virtual BlockType type() const override { return BlockType::BlockHeaderShanghai; }

    FH32 const& withdrawalsRoot() const { return m_withdrawalsRoot; }
    void setWithdrawalsRoot(FH32 const& _wRoot) { m_withdrawalsRoot = spFH32(_wRoot.copy()); }

    static BlockHeaderShanghai const& castFrom(spBlockHeader const& _from);
    static BlockHeaderShanghai& castFrom(BlockHeader& _from);

protected:
    virtual void checkDataScheme(DataObject const&) const override;
    virtual void _fromData(DataObject const&) override;
    virtual size_t _fromRLP(dev::RLP const&) override;
    virtual size_t _rlpHeaderSize() const override { return 17; }

    spFH32 m_withdrawalsRoot;
    BlockHeaderShanghai(){};
};

typedef GCP_SPointer<BlockHeaderShanghai> spBlockHeaderShanghai;


}  // namespace teststruct
