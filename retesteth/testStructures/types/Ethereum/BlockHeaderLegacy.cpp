#include "BlockHeaderLegacy.h"
#include "../../basetypes.h"
#include <libdevcore/Address.h>
#include <libdevcore/RLP.h>
#include <libdevcore/SHA3.h>
#include <retesteth/TestHelper.h>
#include <retesteth/testStructures/Common.h>

using namespace dev;
using namespace test::debug;

namespace test
{
namespace teststruct
{
void BlockHeaderLegacy::fromData(DataObject const& _data)
{
    try
    {
        // Allowed fields for this structure
        REQUIRE_JSONFIELDS(_data, "BlockHeader " + _data.getKey(),
            {
                {"bloom", {{DataType::String}, jsonField::Optional}},
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
                {"rejectedTransactions", {{DataType::Array}, jsonField::Optional}},   // EthGetBlockBy test debug field
                {"seedHash", {{DataType::String}, jsonField::Optional}},         // EthGetBlockBy aleth field
                {"boundary", {{DataType::String}, jsonField::Optional}},         // EthGetBlockBy aleth field
                {"size", {{DataType::String}, jsonField::Optional}},             // EthGetBlockBy field
                {"totalDifficulty", {{DataType::String}, jsonField::Optional}},  // EthGetBlockBy field
                {"transactions", {{DataType::Array}, jsonField::Optional}},      // EthGetBlockBy field
                {"uncles", {{DataType::Array}, jsonField::Optional}}             // EthGetBlockBy field
            });

        string const akey = _data.count("author") ? "author" : _data.count("miner") ? "miner" : "coinbase";
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
            ETH_DC_MESSAGE(DC::TESTLOG, "BlockHeader `mixHash` is not defined. Using default `0x00..00` value!");
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
    }
    catch (std::exception const& _ex)
    {
        throw test::UpwardsException(string("Blockheader parse error: ") + _ex.what());
    }
}

BlockHeaderLegacy::BlockHeaderLegacy(DataObject const& _data)
{
    fromData(_data);
}

BlockHeaderLegacy::BlockHeaderLegacy(dev::RLP const& _rlp)
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
    recalculateHash();
}

spDataObject BlockHeaderLegacy::asDataObject() const
{
    spDataObject out;
    (*out)["bloom"] = m_logsBloom->asString();
    (*out)["coinbase"] = m_author->asString();
    (*out)["difficulty"] = m_difficulty->asString();
    (*out)["extraData"] = m_extraData->asString();
    (*out)["gasLimit"] = m_gasLimit->asString();
    (*out)["gasUsed"] = m_gasUsed->asString();
    (*out)["hash"] = m_hash->asString();
    (*out)["mixHash"] = m_mixHash->asString();
    (*out)["nonce"] = m_nonce->asString();
    (*out)["number"] = m_number->asString();
    (*out)["parentHash"] = m_parentHash->asString();
    (*out)["receiptTrie"] = m_receiptsRoot->asString();
    (*out)["stateRoot"] = m_stateRoot->asString();
    (*out)["timestamp"] = m_timestamp->asString();
    (*out)["transactionsTrie"] = m_transactionsRoot->asString();
    (*out)["uncleHash"] = m_sha3Uncles->asString();
    return out;
}

const RLPStream BlockHeaderLegacy::asRLPStream() const
{
    RLPStream header;
    header.appendList(15);

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
}  // namespace test
