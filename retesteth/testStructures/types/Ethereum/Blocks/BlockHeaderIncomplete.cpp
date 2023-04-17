#include "BlockHeaderIncomplete.h"
#include <retesteth/EthChecks.h>
#include <retesteth/TestHelper.h>
#include <retesteth/testStructures/Common.h>
#include <retesteth/Constants.h>

using namespace dev;
using namespace std;
using namespace dataobject;
using namespace test::teststruct::constnames;

namespace test::teststruct
{
BlockHeaderIncomplete::BlockHeaderIncomplete(DataObject const& _data)
{
    REQUIRE_JSONFIELDS(_data, "BlockHeaderIncomplete " + _data.getKey(),
        {{c_bloom, {{DataType::String}, jsonField::Optional}},
            {c_coinbase, {{DataType::String}, jsonField::Optional}},
            {c_difficulty, {{DataType::String}, jsonField::Optional}},
            {c_extraData, {{DataType::String}, jsonField::Optional}},
            {c_gasLimit, {{DataType::String}, jsonField::Optional}},
            {c_baseFeePerGas, {{DataType::String}, jsonField::Optional}},
            {c_withdrawalsRoot, {{DataType::String}, jsonField::Optional}},
            {c_gasUsed, {{DataType::String}, jsonField::Optional}},
            {c_hash, {{DataType::String}, jsonField::Optional}},
            {c_mixHash, {{DataType::String}, jsonField::Optional}},
            {c_nonce, {{DataType::String}, jsonField::Optional}},
            {c_number, {{DataType::String}, jsonField::Optional}},
            {c_parentHash, {{DataType::String}, jsonField::Optional}},
            {c_receiptTrie, {{DataType::String}, jsonField::Optional}},
            {c_stateRoot, {{DataType::String}, jsonField::Optional}},
            {c_timestamp, {{DataType::String}, jsonField::Optional}},
            {c_transactionsTrie, {{DataType::String}, jsonField::Optional}},
            {"remove", {{DataType::Array}, jsonField::Optional}},
            {"forceNoWithdrawalsRLP", {{DataType::String}, jsonField::Optional}},
            {"uncleHash", {{DataType::String}, jsonField::Optional}}});

    string const& akey = _data.count(c_author) ? c_author : c_coinbase;
    if (_data.count(akey))
        m_author = sFH20(_data.atKey(akey));
    if (_data.count(c_difficulty))
        m_difficulty = sVALUE(_data.atKey(c_difficulty));
    if (_data.count(c_extraData))
        m_extraData = sBYTES(_data.atKey(c_extraData));
    if (_data.count(c_gasLimit))
        m_gasLimit = sVALUE(_data.atKey(c_gasLimit));
    if (_data.count(c_gasUsed))
        m_gasUsed = sVALUE(_data.atKey(c_gasUsed));
    if (_data.count(c_hash))
        m_hash = sFH32(_data.atKey(c_hash));

    string const& bkey = _data.count(c_logsBloom) ? c_logsBloom : c_bloom;
    if (_data.count(bkey))
        m_logsBloom = sFH256(_data.atKey(bkey));

    if (_data.count(c_mixHash))
        m_mixHash = sFH32(_data.atKey(c_mixHash));
    if (_data.count(c_nonce))
        m_nonce = sFH8(_data.atKey(c_nonce));
    if (_data.count(c_number))
        m_number = sVALUE(_data.atKey(c_number));
    if (_data.count(c_parentHash))
        m_parentHash = sFH32(_data.atKey(c_parentHash));

    string const& rkey = _data.count(c_receiptsRoot) ? c_receiptsRoot : c_receiptTrie;
    if (_data.count(rkey))
        m_receiptsRoot = sFH32(_data.atKey(rkey));

    string const& ukey = _data.count(c_sha3Uncles) ? c_sha3Uncles : c_uncleHash;
    if (_data.count(ukey))
        m_sha3Uncles = sFH32(_data.atKey(ukey));

    if (_data.count(c_stateRoot))
        m_stateRoot = sFH32(_data.atKey(c_stateRoot));
    if (_data.count(c_timestamp))
        m_timestamp = sVALUE(_data.atKey(c_timestamp));

    string const& tkey = _data.count(c_transactionsRoot) ? c_transactionsRoot : c_transactionsTrie;
    if (_data.count(tkey))
        m_transactionsRoot = sFH32(_data.atKey(tkey));

    if (_data.count(c_baseFeePerGas))
        m_baseFee = sVALUE(_data.atKey(c_baseFeePerGas));

    if (_data.count(c_withdrawalsRoot))
        m_withdrawalsRoot = sFH32(_data.atKey(c_withdrawalsRoot));

    if (_data.count("remove"))
        test::parseJsonStrValueIntoSet(_data.atKey("remove"), m_removeKeys);

    bool hasAtLeastOneField = !m_author.isEmpty() || !m_difficulty.isEmpty() || !m_extraData.isEmpty() ||
                              !m_gasLimit.isEmpty() || !m_gasUsed.isEmpty() || !m_hash.isEmpty() || !m_logsBloom.isEmpty() ||
                              !m_mixHash.isEmpty() || !m_nonce.isEmpty() || !m_number.isEmpty() || !m_parentHash.isEmpty() ||
                              !m_receiptsRoot.isEmpty() || !m_sha3Uncles.isEmpty() || !m_stateRoot.isEmpty() ||
                              !m_timestamp.isEmpty() || !m_transactionsRoot.isEmpty() || !m_baseFee.isEmpty() ||
                              !m_withdrawalsRoot.isEmpty();

    ETH_ERROR_REQUIRE_MESSAGE(hasAtLeastOneField, "BlockHeaderIncomplete must have at least one field!");
}

spBlockHeader BlockHeaderIncomplete::overwriteBlockHeader(spBlockHeader const& _header) const
{
    spDataObject _overwrite = _header->asDataObject();
    DataObject& overwrite = _overwrite.getContent();
    if (!m_author.isEmpty())
        overwrite[c_author] = m_author->asString();
    if (!m_difficulty.isEmpty())
        overwrite[c_difficulty] = m_difficulty->asString();
    if (!m_extraData.isEmpty())
        overwrite[c_extraData] = m_extraData->asString();
    if (!m_gasLimit.isEmpty())
        overwrite[c_gasLimit] = m_gasLimit->asString();
    if (!m_gasUsed.isEmpty())
        overwrite[c_gasUsed] = m_gasUsed->asString();
    if (!m_logsBloom.isEmpty())
        overwrite[c_logsBloom] = m_logsBloom->asString();
    if (!m_mixHash.isEmpty())
        overwrite[c_mixHash] = m_mixHash->asString();
    if (!m_nonce.isEmpty())
        overwrite[c_nonce] = m_nonce->asString();
    if (!m_number.isEmpty())
        overwrite[c_number] = m_number->asString();
    if (!m_parentHash.isEmpty())
        overwrite[c_parentHash] = m_parentHash->asString();
    if (!m_receiptsRoot.isEmpty())
        overwrite[c_receiptsRoot] = m_receiptsRoot->asString();
    if (!m_sha3Uncles.isEmpty())
        overwrite[c_sha3Uncles] = m_sha3Uncles->asString();
    if (!m_stateRoot.isEmpty())
        overwrite[c_stateRoot] = m_stateRoot->asString();
    if (!m_timestamp.isEmpty())
        overwrite[c_timestamp] = m_timestamp->asString();
    if (!m_transactionsRoot.isEmpty())
        overwrite[c_transactionsRoot] = m_transactionsRoot->asString();
    if (!m_hash.isEmpty())
        overwrite[c_hash] = m_hash->asString();
    if (!m_baseFee.isEmpty())
        overwrite[c_baseFeePerGas] = m_baseFee->asString();
    if (!m_withdrawalsRoot.isEmpty())
        overwrite[c_withdrawalsRoot] = m_withdrawalsRoot->asString();

    overwrite.removeKey("updatePoW");  // deprecated key
    for (auto const& el : m_removeKeys)
        overwrite.removeKey(el);

    return readBlockHeader(overwrite);
}

}  // namespace teststruct
