#include "scheme_blockHeader.h"
using namespace test;

scheme_blockHeader::scheme_blockHeader(DataObject const& _data) : object(_data)
{
    // Traditional Block Header
    if (_data.count("bloom"))
    {
        requireJsonFields(_data, "blockchainTest::blockHeader " + _data.getKey(),
            {{"bloom", {DataType::String}}, {"coinbase", {DataType::String}},
                {"difficulty", {DataType::String}}, {"extraData", {DataType::String}},
                {"gasLimit", {DataType::String}}, {"gasUsed", {DataType::String}},
                {"hash", {DataType::String}}, {"mixHash", {DataType::String}},
                {"nonce", {DataType::String}}, {"number", {DataType::String}},
                {"parentHash", {DataType::String}}, {"receiptTrie", {DataType::String}},
                {"stateRoot", {DataType::String}}, {"timestamp", {DataType::String}},
                {"transactionsTrie", {DataType::String}}, {"uncleHash", {DataType::String}}});
    }
    else
    {
        // Genesis block header
        requireJsonFields(_data, "blockchainTest::blockHeader " + _data.getKey(),
            {{"author", {DataType::String}}, {"difficulty", {DataType::String}},
                {"gasLimit", {DataType::String}}, {"nonce", {DataType::String}},
                {"extraData", {DataType::String}}, {"timestamp", {DataType::String}},
                {"mixHash", {DataType::String}}});
        m_data.renameKey("author", "coinbase");
    }
}
