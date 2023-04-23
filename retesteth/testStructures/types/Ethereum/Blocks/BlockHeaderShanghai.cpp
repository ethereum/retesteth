#include "BlockHeaderShanghai.h"
#include <libdevcore/Address.h>
#include <retesteth/helpers/TestHelper.h>
#include <retesteth/EthChecks.h>
#include <retesteth/testStructures/Common.h>
#include <retesteth/Constants.h>

using namespace std;
using namespace dev;
using namespace test::debug;
using namespace test::teststruct::constnames;

namespace test::teststruct
{

void BlockHeaderShanghai::checkDataScheme(DataObject const& _data) const
{
    // Allowed fields for this structure
    REQUIRE_JSONFIELDS(_data, "BlockHeaderShanghai " + _data.getKey(),
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
            {c_withdrawalsRoot, {{DataType::String}, jsonField::Required}},
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
    m_withdrawalsRoot = sFH32(_data.atKey(c_withdrawalsRoot));
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
    spDataObject out = BlockHeaderMerge::asDataObject();
    (*out)[c_withdrawalsRoot] = m_withdrawalsRoot->asString();
    return out;
}

const RLPStream BlockHeaderShanghai::asRLPStream() const
{
    RLPStream header = BlockHeaderMerge::asRLPStream();
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
