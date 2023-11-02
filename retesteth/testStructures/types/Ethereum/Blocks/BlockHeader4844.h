#pragma once
#include <retesteth/testStructures/basetypes.h>
#include "BlockHeaderShanghai.h"
#include <libdevcore/RLP.h>
#include <libdataobj/DataObject.h>

namespace test::teststruct
{
struct BlockHeader4844 : BlockHeaderShanghai
{
    BlockHeader4844(DataObject const& _in) { fromData(_in); };
    BlockHeader4844(dev::RLP const& _in);

    virtual spDataObject asDataObject() const override;
    virtual dev::RLPStream const asRLPStream() const override;
    virtual BlockType type() const override { return BlockType::BlockHeader4844; }

    VALUE const& excessBlobGas() const { return m_excessBlobGas; }
    void setExcessBlobGas(VALUE const& _v) { m_excessBlobGas = spVALUE(_v.copy()); }
    VALUE const& blobGasUsed() const { return m_blobGasUsed; }
    void setBlobGasUsed(VALUE const& _v) { m_blobGasUsed = spVALUE(_v.copy()); }
    FH32 const& parentBeaconBlockRoot() const { return m_parentBeaconBlockRoot; }
    void setParentBeaconBlockRoot(FH32 const& _v) { m_parentBeaconBlockRoot = spFH32(_v.copy()); }

    static BlockHeader4844 const& castFrom(spBlockHeader const& _from);
    static BlockHeader4844& castFrom(BlockHeader& _from);

protected:
    virtual void checkDataScheme(DataObject const&) const override;
    virtual void _fromData(DataObject const&) override;
    virtual size_t _fromRLP(dev::RLP const&) override;
    virtual size_t _rlpHeaderSize() const override { return 20; }

    spVALUE m_excessBlobGas;
    spVALUE m_blobGasUsed;
    spFH32 m_parentBeaconBlockRoot;
    BlockHeader4844(){};
};

typedef GCP_SPointer<BlockHeader4844> spBlockHeader4844;


}  // namespace teststruct
