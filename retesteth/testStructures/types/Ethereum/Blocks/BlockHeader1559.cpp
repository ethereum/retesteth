#include <libdevcore/Address.h>
#include <retesteth/EthChecks.h>
#include <retesteth/helpers/TestHelper.h>
#include <retesteth/testStructures/Common.h>
#include <retesteth/Constants.h>

using namespace dev;
using namespace std;
using namespace test::debug;
using namespace test::teststruct::constnames;

namespace test::teststruct
{

void BlockHeader1559::checkDataScheme(DataObject const& _data) const
{
    // Allowed fields for this structure
    REQUIRE_JSONFIELDS(_data, "BlockHeader1559 " + _data.getKey(),
        {
            {c_bloom, {{DataType::String}, jsonField::Optional}},
            {c_logsBloom, {{DataType::String}, jsonField::Optional}},
            {c_coinbase, {{DataType::String}, jsonField::Optional}},
            {c_author, {{DataType::String}, jsonField::Optional}},
            {c_miner, {{DataType::String}, jsonField::Optional}},
            {c_difficulty, {{DataType::String}, jsonField::Required}},
            {c_extraData, {{DataType::String}, jsonField::Required}},
            {c_gasLimit, {{DataType::String}, jsonField::Required}},
            {c_baseFeePerGas, {{DataType::String}, jsonField::Required}},
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

void BlockHeader1559::_fromData(DataObject const& _data)
{
    BlockHeaderLegacy::_fromData(_data);
    m_baseFee = sVALUE(_data.atKey(c_baseFeePerGas));
}

size_t BlockHeader1559::_fromRLP(dev::RLP const& _rlp)
{
    // 0 - parentHash           // 8 - number
    // 1 - uncleHash            // 9 - gasLimit
    // 2 - coinbase             // 10 - gasUsed
    // 3 - stateRoot            // 11 - timestamp
    // 4 - transactionTrie      // 12 - extraData
    // 5 - receiptTrie          // 13 - mixHash
    // 6 - bloom                // 14 - nonce
    // 7 - difficulty           // 15 - baseFee
    size_t i = BlockHeaderLegacy::_fromRLP(_rlp);
    m_baseFee = spVALUE(new VALUE(_rlp[i++]));
    return i;
}

BlockHeader1559::BlockHeader1559(dev::RLP const& _rlp)
{
    _fromRLP(_rlp);
    recalculateHash();
}

spDataObject BlockHeader1559::asDataObject() const
{
    spDataObject out = BlockHeaderLegacy::asDataObject();
    (*out)[c_baseFeePerGas] = m_baseFee->asString();
    return out;
}

const RLPStream BlockHeader1559::asRLPStream() const
{
    RLPStream header = BlockHeaderLegacy::asRLPStream();
    header << m_baseFee->asBigInt();
    return header;
}

namespace  {
inline bool isChild(BlockType _t)
{
    // Can't use compareFork function here because of EthereumClassic and custom fork names
    return _t != BlockType::BlockHeader1559 &&
           _t != BlockType::BlockHeaderParis &&
           _t != BlockType::BlockHeaderShanghai &&
           _t != BlockType::BlockHeader4844;
}
}

BlockHeader1559& BlockHeader1559::castFrom(BlockHeader& _from)
{
    try
    {
        if (isChild(_from.type()))
            ETH_FAIL_MESSAGE("BlockHeader1559::castFrom() got wrong block type! `" + BlockHeader::BlockTypeToString(_from.type()));
        return dynamic_cast<BlockHeader1559&>(_from);
    }
    catch (...)
    {
        ETH_FAIL_MESSAGE("BlockHeader1559::castFrom() failed!");
    }
    return dynamic_cast<BlockHeader1559&>(_from);
}

BlockHeader1559 const& BlockHeader1559::castFrom(spBlockHeader const& _from)
{
    try
    {
        if (isChild(_from->type()))
            ETH_FAIL_MESSAGE("BlockHeader1559::castFrom() got wrong block type! `" + BlockHeader::BlockTypeToString(_from->type())
                + "\n" + _from->asDataObject()->asJson());
        return dynamic_cast<BlockHeader1559 const&>(_from.getCContent());
    }
    catch (...)
    {
        ETH_FAIL_MESSAGE("BlockHeader1559::castFrom() failed!");
    }
    spBlockHeader1559 foo(new BlockHeader1559(DataObject()));
    return foo.getCContent();
}

}  // namespace teststruct
