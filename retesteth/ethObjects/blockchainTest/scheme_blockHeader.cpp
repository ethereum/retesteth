#include "scheme_blockHeader.h"
#include "../rpcResponse/scheme_block.h"

using namespace test;

scheme_blockHeader::scheme_blockHeader(DataObject const& _data) : object(_data)
{
    // Traditional Block Header
    if (_data.count("bloom"))
    {
        requireJsonFields(_data, "blockchainTest::scheme_blockHeader filled " + _data.getKey(),
            {
                {"bloom", {{DataType::String}, jsonField::Required}},
                {"coinbase", {{DataType::String}, jsonField::Required}},
                {"difficulty", {{DataType::String}, jsonField::Required}},
                {"extraData", {{DataType::String}, jsonField::Required}},
                {"gasLimit", {{DataType::String}, jsonField::Required}},
                {"gasUsed", {{DataType::String}, jsonField::Required}},
                {"hash", {{DataType::String}, jsonField::Optional}},
                {"mixHash", {{DataType::String}, jsonField::Required}},
                {"nonce", {{DataType::String}, jsonField::Required}},
                {"number", {{DataType::String}, jsonField::Required}},
                {"parentHash", {{DataType::String}, jsonField::Required}},
                {"receiptTrie", {{DataType::String}, jsonField::Required}},
                {"stateRoot", {{DataType::String}, jsonField::Required}},
                {"timestamp", {{DataType::String}, jsonField::Required}},
                {"transactionsTrie", {{DataType::String}, jsonField::Required}},
                {"uncleHash", {{DataType::String}, jsonField::Required}}});
        m_data.performVerifier(ver_ethereumfields);
    }
    else
    {
        // Genesis block header
        requireJsonFields(_data, "blockchainTest::scheme_blockHeader filler " + _data.getKey(),
            {{"author", {DataType::String}}, {"difficulty", {DataType::String}},
                {"gasLimit", {DataType::String}}, {"nonce", {DataType::String}},
                {"extraData", {DataType::String}}, {"timestamp", {DataType::String}},
                {"mixHash", {DataType::String}}});
        m_data.renameKey("author", "coinbase");
    }
    if (!object::validateHash(m_data.atKey("nonce").asString(), 8))
        ETH_ERROR_MESSAGE("Key `nonce` is not hash8 `" + m_data.atKey("nonce").asString() + "`");
}

std::string scheme_blockHeader::hash() const
{
    /// !!! todo solve this. streamBlockHeader rely on m_data contents
    return "0x" + toString(dev::sha3(scheme_RPCBlock::streamBlockHeader(m_data).out()));
}
