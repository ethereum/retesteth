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

    VALUE const& excessDataGas() const { return m_excessDataGas; }
    void setExcessDataGas(VALUE const& _v) { m_excessDataGas = spVALUE(_v.copy()); }
    VALUE const& dataGasUsed() const { return m_dataGasUsed; }
    void setDataGasUsed(VALUE const& _v) { m_dataGasUsed = spVALUE(_v.copy()); }

    static BlockHeader4844 const& castFrom(spBlockHeader const& _from);
    static BlockHeader4844& castFrom(BlockHeader& _from);

protected:
    virtual void checkDataScheme(DataObject const&) const override;
    virtual void _fromData(DataObject const&) override;
    virtual size_t _fromRLP(dev::RLP const&) override;
    virtual size_t _rlpHeaderSize() const override { return 19; }

    spVALUE m_excessDataGas;
    spVALUE m_dataGasUsed;
    BlockHeader4844(){};
};

typedef GCP_SPointer<BlockHeader4844> spBlockHeader4844;


}  // namespace teststruct
