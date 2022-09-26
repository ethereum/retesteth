#pragma once
#include "../../basetypes.h"
#include "BlockHeader.h"
#include <libdevcore/RLP.h>
#include <libdataobj/DataObject.h>
#include <libdataobj/SPointer.h>

namespace test
{
namespace teststruct
{
// Ethereum blockheader description
struct BlockHeaderIncomplete : GCP_SPointerBase
{
    BlockHeaderIncomplete(DataObject const&);
    spBlockHeader overwriteBlockHeader(spBlockHeader const& _header) const;
    bool hasUncleHash() const { return !m_sha3Uncles.isEmpty(); }
    bool hasTransactionHash() const { return !m_transactionsRoot.isEmpty(); }

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

    // 1559 block
    spVALUE m_baseFee;

    // Overwrite fields
    std::set<std::string> m_removeKeys;
};

typedef GCP_SPointer<BlockHeaderIncomplete> spBlockHeaderIncomplete;


}  // namespace teststruct
}  // namespace test
