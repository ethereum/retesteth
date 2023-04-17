#include <libdevcore/Address.h>
#include <retesteth/EthChecks.h>
#include <retesteth/TestHelper.h>
#include <retesteth/testStructures/Common.h>
#include <retesteth/Constants.h>

using namespace dev;
using namespace std;
using namespace test::debug;
using namespace test::teststruct::constnames;

namespace test::teststruct
{

void BlockHeaderLegacy::checkDataScheme(DataObject const& _data) const
{
    // Allowed fields for this structure
    REQUIRE_JSONFIELDS(_data, "BlockHeaderLegacy " + _data.getKey(),
        {
            {c_bloom, {{DataType::String}, jsonField::Optional}},
            {c_logsBloom, {{DataType::String}, jsonField::Optional}},
            {c_coinbase, {{DataType::String}, jsonField::Optional}},
            {c_author, {{DataType::String}, jsonField::Optional}},
            {c_miner, {{DataType::String}, jsonField::Optional}},
            {c_difficulty, {{DataType::String}, jsonField::Required}},
            {c_extraData, {{DataType::String}, jsonField::Required}},
            {c_gasLimit, {{DataType::String}, jsonField::Required}},
            {c_gasUsed, {{DataType::String}, jsonField::Required}},
            {c_hash, {{DataType::String}, jsonField::Optional}},
            {c_mixHash, {{DataType::String}, jsonField::Optional}},
            {c_nonce, {{DataType::String}, jsonField::Optional}},
            {c_number, {{DataType::String}, jsonField::Required}},
            {c_parentHash, {{DataType::String}, jsonField::Required}},
            {c_receiptTrie, {{DataType::String}, jsonField::Optional}},
            {c_receiptsRoot, {{DataType::String}, jsonField::Optional}},
            {c_stateRoot, {{DataType::String}, jsonField::Required}},
            {c_timestamp, {{DataType::String}, jsonField::Required}},
            {c_transactionsTrie, {{DataType::String}, jsonField::Optional}},
            {c_transactionsRoot, {{DataType::String}, jsonField::Optional}},
            {c_sha3Uncles, {{DataType::String}, jsonField::Optional}},
            {c_uncleHash, {{DataType::String}, jsonField::Optional}},
            {"rejectedTransactions", {{DataType::Array}, jsonField::Optional}},   // EthGetBlockBy test debug field
            {"seedHash", {{DataType::String}, jsonField::Optional}},         // EthGetBlockBy aleth field
            {"boundary", {{DataType::String}, jsonField::Optional}},         // EthGetBlockBy aleth field
            {"size", {{DataType::String}, jsonField::Optional}},             // EthGetBlockBy field
            {"totalDifficulty", {{DataType::String}, jsonField::Optional}},  // EthGetBlockBy field
            {"transactions", {{DataType::Array}, jsonField::Optional}},      // EthGetBlockBy field
            {"uncles", {{DataType::Array}, jsonField::Optional}}             // EthGetBlockBy field
        });
}

void BlockHeaderLegacy::_fromData(DataObject const& _data)
{
    string const& akey = _data.count(c_author) ? c_author
                       : _data.count(c_miner) ? c_miner : c_coinbase;
    m_author = sFH20(_data.atKey(akey));
    m_difficulty = sVALUE(_data.atKey(c_difficulty));
    m_extraData = sBYTES(_data.atKey(c_extraData));
    m_gasLimit = sVALUE(_data.atKey(c_gasLimit));
    m_gasUsed = sVALUE(_data.atKey(c_gasUsed));
    if (_data.count(c_hash))
        m_hash = sFH32(_data.atKey(c_hash));
    string const& bkey = _data.count(c_logsBloom) ? c_logsBloom : c_bloom;
    m_logsBloom = sFH256(_data.atKey(bkey));

    if (_data.count(c_nonce))
    {
        m_mixHash = sFH32(_data.atKey(c_mixHash));
        m_nonce = sFH8(_data.atKey(c_nonce));
    }
    else
    {
        ETH_DC_MESSAGE(DC::TESTLOG, "BlockHeader `mixHash` is not defined. Using default `0x00..00` value!");
        m_mixHash = spFH32(FH32::zero().copy());
        m_nonce = spFH8(FH8::zero().copy());
    }

    m_number = sVALUE(_data.atKey(c_number));
    m_parentHash = sFH32(_data.atKey(c_parentHash));
    string const& rkey = _data.count(c_receiptsRoot) ? c_receiptsRoot : c_receiptTrie;
    m_receiptsRoot = sFH32(_data.atKey(rkey));
    string const& ukey = _data.count(c_sha3Uncles) ? c_sha3Uncles : c_uncleHash;
    m_sha3Uncles = sFH32(_data.atKey(ukey));
    m_stateRoot = sFH32(_data.atKey(c_stateRoot));
    m_timestamp = sVALUE(_data.atKey(c_timestamp));
    string const& tkey = _data.count(c_transactionsRoot) ? c_transactionsRoot : c_transactionsTrie;
    m_transactionsRoot = sFH32(_data.atKey(tkey));
}

size_t BlockHeaderLegacy::_fromRLP(dev::RLP const& _rlp)
{
    // 0 - parentHash           // 8 - number
    // 1 - uncleHash            // 9 - gasLimit
    // 2 - coinbase             // 10 - gasUsed
    // 3 - stateRoot            // 11 - timestamp
    // 4 - transactionTrie      // 12 - extraData
    // 5 - receiptTrie          // 13 - mixHash
    // 6 - bloom                // 14 - nonce
    // 7 - difficulty
    size_t i = 0;
    m_parentHash = spFH32(new FH32(_rlp[i++]));
    m_sha3Uncles = spFH32(new FH32(_rlp[i++]));
    m_author = spFH20(new FH20(_rlp[i++]));
    m_stateRoot = spFH32(new FH32(_rlp[i++]));
    m_transactionsRoot = spFH32(new FH32(_rlp[i++]));
    m_receiptsRoot = spFH32(new FH32(_rlp[i++]));
    m_logsBloom = spFH256(new FH256(_rlp[i++]));
    m_difficulty = spVALUE(new VALUE(_rlp[i++]));
    m_number = spVALUE(new VALUE(_rlp[i++]));
    m_gasLimit = spVALUE(new VALUE(_rlp[i++]));
    m_gasUsed = spVALUE(new VALUE(_rlp[i++]));
    m_timestamp = spVALUE(new VALUE(_rlp[i++]));
    m_extraData = spBYTES(new BYTES(_rlp[i++]));
    m_mixHash = spFH32(new FH32(_rlp[i++]));
    m_nonce = spFH8(new FH8(_rlp[i++]));
    return i;
}

BlockHeaderLegacy::BlockHeaderLegacy(dev::RLP const& _rlp)
{
    _fromRLP(_rlp);
    recalculateHash();
}

spDataObject BlockHeaderLegacy::asDataObject() const
{
    spDataObject out;
    (*out)[c_bloom] = m_logsBloom->asString();
    (*out)[c_coinbase] = m_author->asString();
    (*out)[c_difficulty] = m_difficulty->asString();
    (*out)[c_extraData] = m_extraData->asString();
    (*out)[c_gasLimit] = m_gasLimit->asString();
    (*out)[c_gasUsed] = m_gasUsed->asString();
    (*out)[c_hash] = m_hash->asString();
    (*out)[c_mixHash] = m_mixHash->asString();
    (*out)[c_nonce] = m_nonce->asString();
    (*out)[c_number] = m_number->asString();
    (*out)[c_parentHash] = m_parentHash->asString();
    (*out)[c_receiptTrie] = m_receiptsRoot->asString();
    (*out)[c_stateRoot] = m_stateRoot->asString();
    (*out)[c_timestamp] = m_timestamp->asString();
    (*out)[c_transactionsTrie] = m_transactionsRoot->asString();
    (*out)[c_uncleHash] = m_sha3Uncles->asString();
    return out;
}

const RLPStream BlockHeaderLegacy::asRLPStream() const
{
    RLPStream header;
    header.appendList(_rlpHeaderSize());

    header << h256(m_parentHash->asString());
    header << h256(m_sha3Uncles->asString());
    header << Address(m_author->asString());
    header << h256(m_stateRoot->asString());
    header << h256(m_transactionsRoot->asString());
    header << h256(m_receiptsRoot->asString());
    header << h2048(m_logsBloom->asString());
    header << m_difficulty->asBigInt();
    header << m_number->asBigInt();
    header << m_gasLimit->asBigInt();
    header << m_gasUsed->asBigInt();
    header << m_timestamp->asBigInt();
    header << test::sfromHex(m_extraData->asString());
    header << h256(m_mixHash->asString());
    header << h64(m_nonce->asString());
    return header;
}

BlockHeaderLegacy const& BlockHeaderLegacy::castFrom(spBlockHeader const& _from)
{
    try
    {
        if (_from->type() != BlockType::BlockHeaderLegacy)
            ETH_FAIL_MESSAGE("BlockHeaderLegacy::castFrom() got wrong block type!");
        return dynamic_cast<BlockHeaderLegacy const&>(_from.getCContent());
    }
    catch (...)
    {
        ETH_FAIL_MESSAGE("BlockHeaderLegacy::castFrom() failed!");
    }
    spBlockHeaderLegacy foo(new BlockHeaderLegacy(DataObject()));
    return foo.getCContent();
}

}  // namespace teststruct
