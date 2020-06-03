#include "BlockHeaderIncomplete.h"
#include "../../basetypes.h"
#include <libdevcore/Address.h>
#include <retesteth/TestHelper.h>
#include <retesteth/ethObjects/object.h>

using namespace dev;

namespace test
{
namespace teststruct
{
BlockHeaderIncomplete::BlockHeaderIncomplete(DataObject const& _data)
{
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
    if (_data.count("nonce"))
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

    m_hasAtLeastOneField = !m_author.isEmpty() || !m_difficulty.isEmpty() || !m_extraData.isEmpty() || !m_gasLimit.isEmpty() ||
                           !m_gasUsed.isEmpty() || !m_hash.isEmpty() || !m_logsBloom.isEmpty() || !m_mixHash.isEmpty() ||
                           !m_nonce.isEmpty() || !m_number.isEmpty() || !m_parentHash.isEmpty() || !m_receiptsRoot.isEmpty() ||
                           !m_sha3Uncles.isEmpty() || !m_stateRoot.isEmpty() || !m_timestamp.isEmpty() ||
                           !m_transactionsRoot.isEmpty();

    requireJsonFields(_data, "BlockHeader " + _data.getKey(),
        {{"bloom", {{DataType::String}, jsonField::Optional}},
         {"coinbase", {{DataType::String}, jsonField::Optional}},
         {"difficulty", {{DataType::String}, jsonField::Optional}},
         {"extraData", {{DataType::String}, jsonField::Optional}},
         {"gasLimit", {{DataType::String}, jsonField::Optional}},
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
         {"uncleHash", {{DataType::String}, jsonField::Optional}}});
    ETH_ERROR_REQUIRE_MESSAGE(m_hasAtLeastOneField, "BlockHeaderIncomplete must have at least one field!");
}

BlockHeader BlockHeaderIncomplete::overwriteBlockHeader(BlockHeader const& _header) const
{
    DataObject overwrite = _header.asDataObject();
    if (!m_author.isEmpty())
        overwrite["author"] = m_author.getCContent().asString();
    if (!m_difficulty.isEmpty())
        overwrite["difficulty"] = m_difficulty.getCContent().asString();
    if (!m_extraData.isEmpty())
        overwrite["extraData"] = m_extraData.getCContent().asString();
    if (!m_gasLimit.isEmpty())
        overwrite["gasLimit"] = m_gasLimit.getCContent().asString();
    if (!m_gasUsed.isEmpty())
        overwrite["gasUsed"] = m_gasUsed.getCContent().asString();
    if (!m_logsBloom.isEmpty())
        overwrite["logsBloom"] = m_logsBloom.getCContent().asString();
    if (!m_mixHash.isEmpty())
        overwrite["mixHash"] = m_mixHash.getCContent().asString();
    if (!m_nonce.isEmpty())
        overwrite["nonce"] = m_nonce.getCContent().asString();
    if (!m_number.isEmpty())
        overwrite["number"] = m_number.getCContent().asString();
    if (!m_parentHash.isEmpty())
        overwrite["parentHash"] = m_parentHash.getCContent().asString();
    if (!m_receiptsRoot.isEmpty())
        overwrite["receiptsRoot"] = m_receiptsRoot.getCContent().asString();
    if (!m_sha3Uncles.isEmpty())
        overwrite["sha3Uncles"] = m_sha3Uncles.getCContent().asString();
    if (!m_stateRoot.isEmpty())
        overwrite["stateRoot"] = m_stateRoot.getCContent().asString();
    if (!m_timestamp.isEmpty())
        overwrite["timestamp"] = m_timestamp.getCContent().asString();
    if (!m_transactionsRoot.isEmpty())
        overwrite["transactionsRoot"] = m_transactionsRoot.getCContent().asString();
    if (!m_hash.isEmpty())
        overwrite["hash"] = m_hash.getCContent().asString();
    return BlockHeader(overwrite);
}

}  // namespace teststruct
}  // namespace test
