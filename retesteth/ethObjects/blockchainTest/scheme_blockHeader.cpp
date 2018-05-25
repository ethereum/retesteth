#include "scheme_blockHeader.h"
using namespace test;

scheme_blockHeader::scheme_blockHeader(DataObject const& _data) : object(_data)
{
    requireJsonFields(_data, "blockchainTest " + _data.getKey(),
        {{"bloom", {DataType::String}}, {"coinbase", {DataType::String}},
            {"difficulty", {DataType::String}}, {"extraData", {DataType::String}},
            {"gasLimit", {DataType::String}}, {"gasUsed", {DataType::String}},
            {"hash", {DataType::String}}, {"mixHash", {DataType::String}},
            {"nonce", {DataType::String}}, {"number", {DataType::String}},
            {"parentHash", {DataType::String}}, {"receiptTrie", {DataType::String}},
            {"stateRoot", {DataType::String}}, {"timestamp", {DataType::String}},
            {"transactionsTrie", {DataType::String}}, {"uncleHash", {DataType::String}}});
}
