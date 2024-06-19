#pragma once
#include <retesteth/testStructures/basetypes.h>
#include <libdevcore/RLP.h>
#include <libdataobj/DataObject.h>

namespace test::teststruct
{
enum class BlockType
{
    BlockHeaderLegacy,
    BlockHeader1559,
    BlockHeaderParis,
    BlockHeaderShanghai,
    BlockHeader4844,
    BlockHeaderPrague
};

// Ethereum blockheader interface
struct BlockHeader : GCP_SPointerBase
{
    static std::string BlockTypeToString(BlockType _t);
    virtual ~BlockHeader(){/* all smart pointers */};

    virtual spDataObject asDataObject() const = 0;
    virtual dev::RLPStream const asRLPStream() const = 0;
    virtual BlockType type() const = 0;

    bool operator==(BlockHeader const& _rhs) const { return asDataObject() == _rhs.asDataObject(); }
    bool operator!=(BlockHeader const& _rhs) const { return !(*this == _rhs); }

    void recalculateHash();
    bool hasUncles() const;

    // Common
    FH32 const& stateRoot() const { return m_stateRoot; }
    FH32 const& hash() const { return m_hash; }
    FH32 const& parentHash() const { return m_parentHash; }
    VALUE const& number() const { return m_number; }

    void setParentHash(FH32 const& _hash) { m_parentHash = spFH32(_hash.copy()); }
    void setHeaderHash(FH32 const& _hash) { m_hash = spFH32(_hash.copy()); }
    void setStateRoot(FH32 const& _hash) { m_stateRoot = spFH32(_hash.copy()); }
    void setNumber(VALUE const& _number) { m_number = spVALUE(_number.copy()); }

    // Would soon go uncommon
    VALUE const& difficulty() const { return m_difficulty; }
    void setDifficulty(VALUE const& _value) { m_difficulty = spVALUE(_value.copy()); }

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

    void setLogsBloom(FH256 const& _logs) { m_logsBloom = spFH256(_logs.copy()); }
    void setTimestamp(VALUE const& _value) { m_timestamp = spVALUE(_value.copy()); }
    void setTransactionHash(FH32 const& _hash) { m_transactionsRoot = spFH32(_hash.copy()); }
    void setTrReceiptsHash(FH32 const& _hash) { m_receiptsRoot = spFH32(_hash.copy()); }
    void setExtraData(BYTES const& _extraData) { m_extraData = spBYTES(_extraData.copy()); }
    void setUnclesHash(FH32 const& _hash) { m_sha3Uncles = spFH32(_hash.copy()); }
    void setGasUsed(VALUE const& _gasUsed) { m_gasUsed = spVALUE(_gasUsed.copy()); }
    void setGasLimit(VALUE const& _gasLimit) { m_gasLimit = spVALUE(_gasLimit.copy()); }

protected:
    BlockHeader() {}
    void fromData(DataObject const&);
    virtual void checkDataScheme(DataObject const&) const = 0;
    virtual void _fromData(DataObject const&) = 0;
    virtual size_t _fromRLP(dev::RLP const&) = 0;
    virtual size_t _rlpHeaderSize() const = 0;

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
