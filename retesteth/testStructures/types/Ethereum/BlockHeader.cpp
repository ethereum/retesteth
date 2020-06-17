#include "BlockHeader.h"
#include "../../basetypes.h"
#include <libdevcore/Address.h>
#include <libdevcore/CommonIO.h>
#include <libdevcore/RLP.h>
#include <libdevcore/SHA3.h>
#include <retesteth/TestHelper.h>
#include <retesteth/testStructures/Common.h>

using namespace dev;

namespace test
{
namespace teststruct
{
void BlockHeader::fromData(DataObject const& _data)
{
    try
    {
        string const akey = _data.count("author") ? "author" :
                            _data.count("miner") ? "miner" : "coinbase";
        m_author = spFH20(new FH20(_data.atKey(akey)));
        m_difficulty = spVALUE(new VALUE(_data.atKey("difficulty")));
        m_extraData = spBYTES(new BYTES(_data.atKey("extraData")));
        m_gasLimit = spVALUE(new VALUE(_data.atKey("gasLimit")));
        m_gasUsed = spVALUE(new VALUE(_data.atKey("gasUsed")));
        if (_data.count("hash"))
            m_hash = spFH32(new FH32(_data.atKey("hash")));
        string const bkey = _data.count("logsBloom") ? "logsBloom" : "bloom";
        m_logsBloom = spFH256(new FH256(_data.atKey(bkey)));

        if (_data.count("nonce"))
        {
            m_mixHash = spFH32(new FH32(_data.atKey("mixHash")));
            m_nonce = spFH8(new FH8(_data.atKey("nonce")));
        }
        else
        {
            ETH_WARNING_TEST("BlockHeader `mixHash` is not defined. Using default `0x00..00` value!", 6);
            m_mixHash = spFH32(new FH32(FH32::zero()));
            m_nonce = spFH8(new FH8(FH8::zero()));
        }

        m_number = spVALUE(new VALUE(_data.atKey("number")));
        m_parentHash = spFH32(new FH32(_data.atKey("parentHash")));
        string const rkey = _data.count("receiptsRoot") ? "receiptsRoot" : "receiptTrie";
        m_receiptsRoot = spFH32(new FH32(_data.atKey(rkey)));
        string const ukey = _data.count("sha3Uncles") ? "sha3Uncles" : "uncleHash";
        m_sha3Uncles = spFH32(new FH32(_data.atKey(ukey)));
        m_stateRoot = spFH32(new FH32(_data.atKey("stateRoot")));
        m_timestamp = spVALUE(new VALUE(_data.atKey("timestamp")));
        string const tkey = _data.count("transactionsRoot") ? "transactionsRoot" : "transactionsTrie";
        m_transactionsRoot = spFH32(new FH32(_data.atKey(tkey)));

        // Manual hash calculation
        if (m_hash.isEmpty())
            recalculateHash();

        // Allowed fields for this structure
        requireJsonFields(_data, "BlockHeader " + _data.getKey(),
            {{"bloom", {{DataType::String}, jsonField::Optional}},
             {"logsBloom", {{DataType::String}, jsonField::Optional}},
             {"coinbase", {{DataType::String}, jsonField::Optional}},
             {"author", {{DataType::String}, jsonField::Optional}},
             {"miner", {{DataType::String}, jsonField::Optional}},
             {"difficulty", {{DataType::String}, jsonField::Required}},
             {"extraData", {{DataType::String}, jsonField::Required}},
             {"gasLimit", {{DataType::String}, jsonField::Required}},
             {"gasUsed", {{DataType::String}, jsonField::Required}},
             {"hash", {{DataType::String}, jsonField::Optional}},
             {"mixHash", {{DataType::String}, jsonField::Optional}},
             {"nonce", {{DataType::String}, jsonField::Optional}},
             {"number", {{DataType::String}, jsonField::Required}},
             {"parentHash", {{DataType::String}, jsonField::Required}},
             {"receiptTrie", {{DataType::String}, jsonField::Optional}},
             {"receiptsRoot", {{DataType::String}, jsonField::Optional}},
             {"stateRoot", {{DataType::String}, jsonField::Required}},
             {"timestamp", {{DataType::String}, jsonField::Required}},
             {"transactionsTrie", {{DataType::String}, jsonField::Optional}},
             {"transactionsRoot", {{DataType::String}, jsonField::Optional}},
             {"sha3Uncles", {{DataType::String}, jsonField::Optional}},
             {"uncleHash", {{DataType::String}, jsonField::Optional}},
             {"seedHash", {{DataType::String}, jsonField::Optional}},            // EthGetBlockBy aleth field
             {"boundary", {{DataType::String}, jsonField::Optional}},            // EthGetBlockBy aleth field
             {"size", {{DataType::String}, jsonField::Optional}},               // EthGetBlockBy field
             {"totalDifficulty", {{DataType::String}, jsonField::Optional}},    // EthGetBlockBy field
             {"transactions", {{DataType::Array}, jsonField::Optional}},        // EthGetBlockBy field
             {"uncles", {{DataType::Array}, jsonField::Optional}}               // EthGetBlockBy field
                          });
    }
    catch (std::exception const& _ex)
    {
        throw test::UpwardsException(string("Blockheader parse error: ") + _ex.what());
    }
}

BlockHeader::BlockHeader(DataObject const& _data)
{
    fromData(_data);
}

BlockHeader::BlockHeader(dev::RLP const& _rlp)
{
    DataObject init;
    // 0 - parentHash           // 8 - number
    // 1 - uncleHash            // 9 - gasLimit
    // 2 - coinbase             // 10 - gasUsed
    // 3 - stateRoot            // 11 - timestamp
    // 4 - transactionTrie      // 12 - extraData
    // 5 - receiptTrie          // 13 - mixHash
    // 6 - bloom                // 14 - nonce
    // 7 - difficulty
    size_t i = 0;
    init["parentHash"] = rlpToString(_rlp[i++]);
    init["uncleHash"] = rlpToString(_rlp[i++]);
    init["coinbase"] = rlpToString(_rlp[i++]);
    init["stateRoot"] = rlpToString(_rlp[i++]);
    init["transactionsTrie"] = rlpToString(_rlp[i++]);
    init["receiptTrie"] = rlpToString(_rlp[i++]);
    init["bloom"] = rlpToString(_rlp[i++]);
    init["difficulty"] = rlpToString(_rlp[i++]);
    init["number"] = rlpToString(_rlp[i++]);
    init["gasLimit"] = rlpToString(_rlp[i++]);
    init["gasUsed"] = rlpToString(_rlp[i++]);
    init["timestamp"] = rlpToString(_rlp[i++]);
    init["extraData"] = rlpToString(_rlp[i++], 0);
    init["mixHash"] = rlpToString(_rlp[i++]);
    init["nonce"] = rlpToString(_rlp[i++]);
    fromData(init);
}

const DataObject BlockHeader::asDataObject() const
{
    DataObject out;
    out["bloom"] = m_logsBloom.getCContent().asString();
    out["coinbase"] = m_author.getCContent().asString();
    out["difficulty"] = m_difficulty.getCContent().asString();
    out["extraData"] = m_extraData.getCContent().asString();
    out["gasLimit"] = m_gasLimit.getCContent().asString();
    out["gasUsed"] = m_gasUsed.getCContent().asString();
    out["hash"] = m_hash.getCContent().asString();
    out["mixHash"] = m_mixHash.getCContent().asString();
    out["nonce"] = m_nonce.getCContent().asString();
    out["number"] = m_number.getCContent().asString();
    out["parentHash"] = m_parentHash.getCContent().asString();
    out["receiptTrie"] = m_receiptsRoot.getCContent().asString();
    out["stateRoot"] = m_stateRoot.getCContent().asString();
    out["timestamp"] = m_timestamp.getCContent().asString();
    out["transactionsTrie"] = m_transactionsRoot.getCContent().asString();
    out["uncleHash"] = m_sha3Uncles.getCContent().asString();
    return out;
}

const RLPStream BlockHeader::asRLPStream() const
{
    RLPStream header;
    header.appendList(15);

    header << h256(m_parentHash.getCContent().asString());
    header << h256(m_sha3Uncles.getCContent().asString());
    header << Address(m_author.getCContent().asString());
    header << h256(m_stateRoot.getCContent().asString());
    header << h256(m_transactionsRoot.getCContent().asString());
    header << h256(m_receiptsRoot.getCContent().asString());
    header << h2048(m_logsBloom.getCContent().asString());
    header << m_difficulty.getCContent().asU256();
    header << m_number.getCContent().asU256();
    header << m_gasLimit.getCContent().asU256();
    header << m_gasUsed.getCContent().asU256();
    header << m_timestamp.getCContent().asU256();
    header << test::sfromHex(m_extraData.getCContent().asString());
    header << h256(m_mixHash.getCContent().asString());
    header << h64(m_nonce.getCContent().asString());
    return header;
}

bool BlockHeader::operator==(BlockHeader const& _rhs) const
{
    return asDataObject() == _rhs.asDataObject();
}

void BlockHeader::recalculateHash()
{
    m_hash = spFH32(new FH32("0x" + toString(dev::sha3(asRLPStream().out()))));
}

bool BlockHeader::hasUncles() const
{
    return m_sha3Uncles.getCContent().asString() != "0x1dcc4de8dec75d7aab85b567b6ccd41ad312451b948a7413f0a142fd40d49347";
}

}  // namespace teststruct
}  // namespace test
