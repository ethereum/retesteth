#pragma once
#include <retesteth/testStructures/basetypes.h>
#include "BlockHeader4844.h"
#include <libdevcore/RLP.h>
#include <libdataobj/DataObject.h>

namespace test::teststruct
{
struct BlockHeaderPrague : BlockHeader4844
{
    BlockHeaderPrague(DataObject const& _in) { fromData(_in); };
    BlockHeaderPrague(dev::RLP const& _in);

    virtual spDataObject asDataObject() const override;
    virtual dev::RLPStream const asRLPStream() const override;
    virtual BlockType type() const override { return BlockType::BlockHeaderPrague; }

    static BlockHeaderPrague const& castFrom(spBlockHeader const& _from);
    static BlockHeaderPrague& castFrom(BlockHeader& _from);

public:
    FH32 const& requestsHash() const { return m_requestsHash; }
    void setRequestsHash(FH32 const& _v) { m_requestsHash = spFH32(_v.copy()); }

protected:
    virtual void checkDataScheme(DataObject const&) const override;
    virtual void _fromData(DataObject const&) override;
    virtual size_t _fromRLP(dev::RLP const&) override;
    virtual size_t _rlpHeaderSize() const override { return 21; }

    spFH32 m_requestsHash;
    BlockHeaderPrague(){};
};

typedef GCP_SPointer<BlockHeaderPrague> spBlockHeaderPrague;


}  // namespace teststruct
