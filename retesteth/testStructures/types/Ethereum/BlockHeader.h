#pragma once
#include "../../basetypes.h"
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
struct BlockHeader : GCP_SPointerBase
{
    BlockHeader(DataObject const&);
    DataObject const asDataObject() const;
    dev::RLPStream const asRLPStream() const;
    bool operator==(BlockHeader const& _rhs) const;

    FH32 const& stateRoot() const { return m_stateRoot.getCContent(); }
    FH32 const& transactionRoot() const { return m_transactionsRoot.getCContent(); }
    FH32 const& uncleHash() const { return m_sha3Uncles.getCContent(); }
    FH32 const& hash() const { return m_hash.getCContent(); }
    VALUE const& number() const { return m_number.getCContent(); }
    VALUE const& timestamp() const { return m_timestamp.getCContent(); }
    FH20 const& author() const { return m_author.getCContent(); }

private:
    BlockHeader() {}
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
};

typedef GCP_SPointer<BlockHeader> spBlockHeader;


}  // namespace teststruct
}  // namespace test
