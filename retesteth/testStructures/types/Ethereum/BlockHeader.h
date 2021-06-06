#pragma once
#include "../../basetypes.h"
#include <libdevcore/CommonIO.h>
#include <libdevcore/RLP.h>
#include <libdevcore/SHA3.h>
#include <retesteth/dataObject/DataObject.h>
#include <retesteth/dataObject/SPointer.h>
using namespace dataobject;
using namespace test::teststruct;

namespace test
{
namespace teststruct
{
enum class BlockType
{
    BlockHeaderLegacy,
    BlockHeader1559
};

// Ethereum blockheader interface
struct BlockHeader : GCP_SPointerBase
{
    virtual ~BlockHeader(){/* all smart pointers */};

    virtual DataObject const asDataObject() const = 0;
    virtual dev::RLPStream const asRLPStream() const = 0;
    virtual BlockType type() const = 0;

    bool operator==(BlockHeader const& _rhs) const { return asDataObject() == _rhs.asDataObject(); }
    bool operator!=(BlockHeader const& _rhs) const { return !(*this == _rhs); }

    void recalculateHash() { m_hash = spFH32(new FH32("0x" + dev::toString(dev::sha3(asRLPStream().out())))); }
    bool hasUncles() const
    {
        return m_sha3Uncles->asString() != "0x1dcc4de8dec75d7aab85b567b6ccd41ad312451b948a7413f0a142fd40d49347";
    }

    // Common
    FH32 const& stateRoot() const { return m_stateRoot; }
    FH32 const& hash() const { return m_hash; }
    FH32 const& parentHash() const { return m_parentHash; }
    VALUE const& number() const { return m_number; }

    void setParentHash(FH32 const& _hash) { m_parentHash = spFH32(new FH32(_hash)); }
    void setHeaderHash(FH32 const& _hash) { m_hash = spFH32(new FH32(_hash)); }
    void setStateRoot(FH32 const& _hash) { m_stateRoot = spFH32(new FH32(_hash)); }
    void setNumber(VALUE const& _number) { m_number = spVALUE(new VALUE(_number)); }

    // Would soon go uncommon
    VALUE const& difficulty() const { return m_difficulty; }
    void setDifficulty(VALUE const& _value) { m_difficulty = spVALUE(new VALUE(_value.asU256())); }

    // Classic Ethereum blockheader fields
    FH32 const& receiptTrie() const { return m_receiptsRoot; }
    FH32 const& transactionRoot() const { return m_transactionsRoot; }
    FH32 const& uncleHash() const { return m_sha3Uncles; }
    VALUE const& timestamp() const { return m_timestamp; }
    FH20 const& author() const { return m_author; }
    VALUE const& gasUsed() const { return m_gasUsed; }
    BYTES const& extraData() const { return m_extraData; }
    FH8 const& nonce() const { return m_nonce; }
    FH32 const& mixHash() const { return m_mixHash; }
    VALUE const& gasLimit() const { return m_gasLimit; }

    void setLogsBloom(FH256 const& _logs) { m_logsBloom = spFH256(new FH256(_logs)); }
    void setTimestamp(VALUE const& _value) { m_timestamp = spVALUE(new VALUE(_value.asU256())); }
    void setTransactionHash(FH32 const& _hash) { m_transactionsRoot = spFH32(new FH32(_hash)); }
    void setTrReceiptsHash(FH32 const& _hash) { m_receiptsRoot = spFH32(new FH32(_hash)); }
    void setExtraData(BYTES const& _extraData) { m_extraData = spBYTES(new BYTES(_extraData)); }
    void setUnclesHash(FH32 const& _hash) { m_sha3Uncles = spFH32(new FH32(_hash)); }
    void setGasUsed(VALUE const& _gasUsed) { m_gasUsed = spVALUE(new VALUE(_gasUsed)); }
    void setGasLimit(VALUE const& _gasLimit) { m_gasLimit = spVALUE(new VALUE(_gasLimit)); }

protected:
    BlockHeader() {}
    virtual void fromData(DataObject const&) = 0;

    // Common
    spFH32 m_stateRoot;
    spVALUE m_number;
    spFH32 m_hash;
    spFH32 m_parentHash;

    // Would soon go uncommon
    spVALUE m_difficulty;

    // Classic ethereum blockheader fields
    spFH20 m_author;
    spBYTES m_extraData;
    spVALUE m_gasUsed;
    spVALUE m_gasLimit;
    spFH256 m_logsBloom;
    spFH32 m_mixHash;
    spFH8 m_nonce;
    spFH32 m_receiptsRoot;
    spFH32 m_sha3Uncles;
    spVALUE m_timestamp;
    spFH32 m_transactionsRoot;
};

typedef GCP_SPointer<BlockHeader> spBlockHeader;


}  // namespace teststruct
}  // namespace test
