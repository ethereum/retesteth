#include "BlockHeader4844.h"
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

void BlockHeader4844::checkDataScheme(DataObject const& _data) const
{
    // Allowed fields for this structure
    REQUIRE_JSONFIELDS(_data, "BlockHeader4844 " + _data.getKey(),
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
            {c_blobGasUsed, {{DataType::String}, jsonField::Required}},
            {c_excessBlobGas, {{DataType::String}, jsonField::Required}},
            {c_parentBeaconBlockRoot, {{DataType::String}, jsonField::Required}},
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

void BlockHeader4844::_fromData(DataObject const& _data)
{
    BlockHeaderShanghai::_fromData(_data);
    m_blobGasUsed = sVALUE(_data.atKey(c_blobGasUsed));
    m_excessBlobGas = sVALUE(_data.atKey(c_excessBlobGas));
    m_parentBeaconBlockRoot = sFH32(_data.atKey(c_parentBeaconBlockRoot));
}

size_t BlockHeader4844::_fromRLP(dev::RLP const& _rlp)
{
    // 0 - parentHash           // 8 - number
    // 1 - uncleHash            // 9 - gasLimit
    // 2 - coinbase             // 10 - gasUsed
    // 3 - stateRoot            // 11 - timestamp
    // 4 - transactionTrie      // 12 - extraData
    // 5 - receiptTrie          // 13 - mixHash
    // 6 - bloom                // 14 - nonce
    // 7 - difficulty            // 15 - baseFee
    // 16 - withdrawals root    // 17 - excessBlobGas
    // 18 - BlobGasUsed         // 19 - beaconRoot
    size_t i = BlockHeaderShanghai::_fromRLP(_rlp);
    m_blobGasUsed = sVALUE(_rlp[i++]);
    m_excessBlobGas = sVALUE(_rlp[i++]);
    m_parentBeaconBlockRoot = sFH32(_rlp[i++]);
    return i;
}

BlockHeader4844::BlockHeader4844(dev::RLP const& _rlp)
{
    _fromRLP(_rlp);
    recalculateHash();
}

spDataObject BlockHeader4844::asDataObject() const
{
    spDataObject out = BlockHeaderShanghai::asDataObject();
    (*out)[c_blobGasUsed] = m_blobGasUsed->asString();
    (*out)[c_excessBlobGas] = m_excessBlobGas->asString();
    (*out)[c_parentBeaconBlockRoot] = m_parentBeaconBlockRoot->asString();
    return out;
}

const RLPStream BlockHeader4844::asRLPStream() const
{
    RLPStream header = BlockHeaderShanghai::asRLPStream();
    header << m_blobGasUsed->serializeRLP();
    header << m_excessBlobGas->serializeRLP();
    header << m_parentBeaconBlockRoot->serializeRLP();
    return header;
}


namespace  {
inline bool isChild(BlockType _t)
{
    // Can't use compareFork function here because of EthereumClassic and custom fork names
    return _t != BlockType::BlockHeader4844;
}
}

BlockHeader4844& BlockHeader4844::castFrom(BlockHeader& _from)
{
    try
    {
        if (isChild(_from.type()))
            ETH_FAIL_MESSAGE("BlockHeader4844::castFrom() got wrong block type!");
        return dynamic_cast<BlockHeader4844&>(_from);
    }
    catch (...)
    {
        ETH_FAIL_MESSAGE("BlockHeader4844::castFrom() failed!");
    }
    return dynamic_cast<BlockHeader4844&>(_from);
}

BlockHeader4844 const& BlockHeader4844::castFrom(spBlockHeader const& _from)
{
    try
    {
        if (isChild(_from->type()))
            ETH_FAIL_MESSAGE("BlockHeader4844::castFrom() got wrong block type!");
        return dynamic_cast<BlockHeader4844 const&>(_from.getCContent());
    }
    catch (...)
    {
        ETH_FAIL_MESSAGE("BlockHeader4844::castFrom() failed!");
    }
    spBlockHeader4844 foo(new BlockHeader4844(DataObject()));
    return foo.getCContent();
}

}  // namespace teststruct
