#include "BlockHeaderIncomplete.h"
#include "../../basetypes.h"
#include <libdevcore/Address.h>
#include <retesteth/EthChecks.h>
#include <retesteth/TestHelper.h>
#include <retesteth/testStructures/Common.h>

using namespace dev;
using namespace std;
using namespace dataobject;

namespace test::teststruct
{
BlockHeaderIncomplete::BlockHeaderIncomplete(DataObject const& _data)
{
    REQUIRE_JSONFIELDS(_data, "BlockHeaderIncomplete " + _data.getKey(),
        {{"bloom", {{DataType::String}, jsonField::Optional}},
            {"coinbase", {{DataType::String}, jsonField::Optional}},
            {"difficulty", {{DataType::String}, jsonField::Optional}},
            {"extraData", {{DataType::String}, jsonField::Optional}},
            {"gasLimit", {{DataType::String}, jsonField::Optional}},
            {"baseFeePerGas", {{DataType::String}, jsonField::Optional}},
            {"gasUsed", {{DataType::String}, jsonField::Optional}},
            {"hash", {{DataType::String}, jsonField::Optional}},
            {"mixHash", {{DataType::String}, jsonField::Optional}},
            {"nonce", {{DataType::String}, jsonField::Optional}},
            {"number", {{DataType::String}, jsonField::Optional}},
            {"parentHash", {{DataType::String}, jsonField::Optional}},
            {"receiptTrie", {{DataType::String}, jsonField::Optional}},
            {"stateRoot", {{DataType::String}, jsonField::Optional}},
            {"timestamp", {{DataType::String}, jsonField::Optional}},
            {"transactionsTrie", {{DataType::String}, jsonField::Optional}},
            {"remove", {{DataType::Array}, jsonField::Optional}},
            {"uncleHash", {{DataType::String}, jsonField::Optional}}});

    string const akey = _data.count("author") ? "author" : "coinbase";
    if (_data.count(akey))
        m_author = spFH20(new FH20(_data.atKey(akey)));
    if (_data.count("difficulty"))
        m_difficulty = spVALUE(new VALUE(_data.atKey("difficulty")));
    if (_data.count("extraData"))
        m_extraData = spBYTES(new BYTES(_data.atKey("extraData")));
    if (_data.count("gasLimit"))
        m_gasLimit = spVALUE(new VALUE(_data.atKey("gasLimit")));
    if (_data.count("gasUsed"))
        m_gasUsed = spVALUE(new VALUE(_data.atKey("gasUsed")));
    if (_data.count("hash"))
        m_hash = spFH32(new FH32(_data.atKey("hash")));

    string const bkey = _data.count("logsBloom") ? "logsBloom" : "bloom";
    if (_data.count(bkey))
        m_logsBloom = spFH256(new FH256(_data.atKey(bkey)));

    if (_data.count("mixHash"))
        m_mixHash = spFH32(new FH32(_data.atKey("mixHash")));
    if (_data.count("nonce"))
        m_nonce = spFH8(new FH8(_data.atKey("nonce")));
    if (_data.count("number"))
        m_number = spVALUE(new VALUE(_data.atKey("number")));
    if (_data.count("parentHash"))
        m_parentHash = spFH32(new FH32(_data.atKey("parentHash")));

    string const rkey = _data.count("receiptsRoot") ? "receiptsRoot" : "receiptTrie";
    if (_data.count(rkey))
        m_receiptsRoot = spFH32(new FH32(_data.atKey(rkey)));

    string const ukey = _data.count("sha3Uncles") ? "sha3Uncles" : "uncleHash";
    if (_data.count(ukey))
        m_sha3Uncles = spFH32(new FH32(_data.atKey(ukey)));

    if (_data.count("stateRoot"))
        m_stateRoot = spFH32(new FH32(_data.atKey("stateRoot")));
    if (_data.count("timestamp"))
        m_timestamp = spVALUE(new VALUE(_data.atKey("timestamp")));

    string const tkey = _data.count("transactionsRoot") ? "transactionsRoot" : "transactionsTrie";
    if (_data.count(tkey))
        m_transactionsRoot = spFH32(new FH32(_data.atKey(tkey)));

    if (_data.count("baseFeePerGas"))
        m_baseFee = spVALUE(new VALUE(_data.atKey("baseFeePerGas")));

    if (_data.count("remove"))
        test::parseJsonStrValueIntoSet(_data.atKey("remove"), m_removeKeys);

    bool hasAtLeastOneField = !m_author.isEmpty() || !m_difficulty.isEmpty() || !m_extraData.isEmpty() ||
                              !m_gasLimit.isEmpty() || !m_gasUsed.isEmpty() || !m_hash.isEmpty() || !m_logsBloom.isEmpty() ||
                              !m_mixHash.isEmpty() || !m_nonce.isEmpty() || !m_number.isEmpty() || !m_parentHash.isEmpty() ||
                              !m_receiptsRoot.isEmpty() || !m_sha3Uncles.isEmpty() || !m_stateRoot.isEmpty() ||
                              !m_timestamp.isEmpty() || !m_transactionsRoot.isEmpty() || !m_baseFee.isEmpty();

    ETH_ERROR_REQUIRE_MESSAGE(hasAtLeastOneField, "BlockHeaderIncomplete must have at least one field!");
}

spBlockHeader BlockHeaderIncomplete::overwriteBlockHeader(spBlockHeader const& _header) const
{
    spDataObject _overwrite = _header->asDataObject();
    DataObject& overwrite = _overwrite.getContent();
    if (!m_author.isEmpty())
        overwrite["author"] = m_author->asString();
    if (!m_difficulty.isEmpty())
        overwrite["difficulty"] = m_difficulty->asString();
    if (!m_extraData.isEmpty())
        overwrite["extraData"] = m_extraData->asString();
    if (!m_gasLimit.isEmpty())
        overwrite["gasLimit"] = m_gasLimit->asString();
    if (!m_gasUsed.isEmpty())
        overwrite["gasUsed"] = m_gasUsed->asString();
    if (!m_logsBloom.isEmpty())
        overwrite["logsBloom"] = m_logsBloom->asString();
    if (!m_mixHash.isEmpty())
        overwrite["mixHash"] = m_mixHash->asString();
    if (!m_nonce.isEmpty())
        overwrite["nonce"] = m_nonce->asString();
    if (!m_number.isEmpty())
        overwrite["number"] = m_number->asString();
    if (!m_parentHash.isEmpty())
        overwrite["parentHash"] = m_parentHash->asString();
    if (!m_receiptsRoot.isEmpty())
        overwrite["receiptsRoot"] = m_receiptsRoot->asString();
    if (!m_sha3Uncles.isEmpty())
        overwrite["sha3Uncles"] = m_sha3Uncles->asString();
    if (!m_stateRoot.isEmpty())
        overwrite["stateRoot"] = m_stateRoot->asString();
    if (!m_timestamp.isEmpty())
        overwrite["timestamp"] = m_timestamp->asString();
    if (!m_transactionsRoot.isEmpty())
        overwrite["transactionsRoot"] = m_transactionsRoot->asString();
    if (!m_hash.isEmpty())
        overwrite["hash"] = m_hash->asString();
    if (!m_baseFee.isEmpty())
        overwrite["baseFeePerGas"] = m_baseFee->asString();
    overwrite.removeKey("updatePoW");  // deprecated key
    for (auto const& el : m_removeKeys)
        overwrite.removeKey(el);

    return readBlockHeader(overwrite);
}

}  // namespace teststruct
