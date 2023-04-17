#include "BlockHeaderShanghai.h"
#include <libdevcore/Address.h>
#include <retesteth/TestHelper.h>
#include <retesteth/EthChecks.h>
#include <retesteth/testStructures/Common.h>

using namespace std;
using namespace dev;
using namespace test::debug;

namespace test::teststruct
{

void BlockHeaderShanghai::checkDataScheme(DataObject const& _data)
{
    // Allowed fields for this structure
    REQUIRE_JSONFIELDS(_data, "BlockHeader1559 " + _data.getKey(),
        {
            {"bloom", {{DataType::String}, jsonField::Optional}},
            {"logsBloom", {{DataType::String}, jsonField::Optional}},
            {"coinbase", {{DataType::String}, jsonField::Optional}},
            {"author", {{DataType::String}, jsonField::Optional}},
            {"miner", {{DataType::String}, jsonField::Optional}},
            {"difficulty", {{DataType::String}, jsonField::Required}},
            {"extraData", {{DataType::String}, jsonField::Required}},
            {"gasLimit", {{DataType::String}, jsonField::Required}},
            {"baseFeePerGas", {{DataType::String}, jsonField::Required}},
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
            {"withdrawalsRoot", {{DataType::String}, jsonField::Required}},
            {"rejectedTransactions", {{DataType::Array}, jsonField::Optional}},   // EthGetBlockBy test debug field
            {"seedHash", {{DataType::String}, jsonField::Optional}},         // EthGetBlockBy aleth field
            {"boundary", {{DataType::String}, jsonField::Optional}},         // EthGetBlockBy aleth field
            {"size", {{DataType::String}, jsonField::Optional}},             // EthGetBlockBy field
            {"totalDifficulty", {{DataType::String}, jsonField::Optional}},  // EthGetBlockBy field
            {"transactions", {{DataType::Array}, jsonField::Optional}},      // EthGetBlockBy field
            {"uncles", {{DataType::Array}, jsonField::Optional}},            // EthGetBlockBy field
            {"withdrawals", {{DataType::Array}, jsonField::Optional}}        // EthGetBlockBy field
        });
}

void BlockHeaderShanghai::_fromData(DataObject const& _data)
{
    BlockHeaderMerge::_fromData(_data);
    m_withdrawalsRoot = spFH32(new FH32(_data.atKey("withdrawalsRoot")));
}

void BlockHeaderShanghai::fromData(DataObject const& _data)
{
    try
    {
        checkDataScheme(_data);
        _fromData(_data);
        if (m_hash.isEmpty())
            recalculateHash();
    }
    catch (std::exception const& _ex)
    {
        throw test::UpwardsException(string("BlockheaderShanghai parse error: ") + _ex.what());
    }
}

size_t BlockHeaderShanghai::_fromRLP(dev::RLP const& _rlp)
{
    // 0 - parentHash           // 8 - number
    // 1 - uncleHash            // 9 - gasLimit
    // 2 - coinbase             // 10 - gasUsed
    // 3 - stateRoot            // 11 - timestamp
    // 4 - transactionTrie      // 12 - extraData
    // 5 - receiptTrie          // 13 - mixHash
    // 6 - bloom                // 14 - nonce
    // 7 - difficulty           // 15 - baseFee
    // 16 - withdrawals root
    size_t i = BlockHeaderMerge::_fromRLP(_rlp);
    m_withdrawalsRoot = spFH32(new FH32(_rlp[i++]));
    return i;
}

BlockHeaderShanghai::BlockHeaderShanghai(dev::RLP const& _rlp)
{
    _fromRLP(_rlp);
    recalculateHash();
}

spDataObject BlockHeaderShanghai::asDataObject() const
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
    (*out)["baseFeePerGas"] = m_baseFee->asString();
    (*out)["withdrawalsRoot"] = m_withdrawalsRoot->asString();
    return out;
}

const RLPStream BlockHeaderShanghai::asRLPStream() const
{
    RLPStream header;
    header.appendList(17);

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
    header << m_baseFee->asBigInt();
    header << h256(m_withdrawalsRoot->asString());
    return header;
}

BlockHeaderShanghai& BlockHeaderShanghai::castFrom(BlockHeader& _from)
{
    try
    {
        if (_from.type() != BlockType::BlockHeaderShanghai)
            ETH_FAIL_MESSAGE("BlockHeaderShanghai::castFrom() got wrong block type!");
        return dynamic_cast<BlockHeaderShanghai&>(_from);
    }
    catch (...)
    {
        ETH_FAIL_MESSAGE("BlockHeaderShanghai::castFrom() failed!");
    }
    return dynamic_cast<BlockHeaderShanghai&>(_from);
}

BlockHeaderShanghai const& BlockHeaderShanghai::castFrom(spBlockHeader const& _from)
{
    try
    {
        if (_from->type() != BlockType::BlockHeaderShanghai)
            ETH_FAIL_MESSAGE("BlockHeaderShanghai::castFrom() got wrong block type!");
        return dynamic_cast<BlockHeaderShanghai const&>(_from.getCContent());
    }
    catch (...)
    {
        ETH_FAIL_MESSAGE("BlockHeaderShanghai::castFrom() failed!");
    }
    spBlockHeaderShanghai foo(new BlockHeaderShanghai(DataObject()));
    return foo.getCContent();
}

}  // namespace teststruct
