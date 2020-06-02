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
struct BlockHeaderIncomplete : GCP_SPointerBase
{
    BlockHeaderIncomplete(DataObject const&);
    bool hasAtLeastOneField() const { return m_hasAtLeastOneField; }
    BlockHeader overwriteBlockHeader(BlockHeader const& _header) const;

private:
    BlockHeaderIncomplete() {}
    spFH20 m_author;
    spVALUE m_difficulty;
    spBYTES m_extraData;
    spVALUE m_gasLimit;
    spVALUE m_gasUsed;
    spFH32 m_hash;
    spFH256 m_logsBloom;
    spFH32 m_mixHash;
    spFH8 m_nonce;
    spVALUE m_number;
    spFH32 m_parentHash;
    spFH32 m_receiptsRoot;
    spFH32 m_sha3Uncles;
    spFH32 m_stateRoot;
    spVALUE m_timestamp;
    spFH32 m_transactionsRoot;
    bool m_hasAtLeastOneField = false;
};

typedef GCP_SPointer<BlockHeaderIncomplete> spBlockHeaderIncomplete;


}  // namespace teststruct
}  // namespace test
