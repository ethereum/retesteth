#pragma once
#include "../object.h"

#include <libdevcore/Address.h>
#include <libdevcore/RLP.h>
#include <libdevcore/SHA3.h>
#include <retesteth/TestHelper.h>

namespace test
{
class scheme_toolResponse : public object
{
public:
    scheme_toolResponse(DataObject const& _toolResponse) : object(_toolResponse)
    {
        requireJsonFields(_toolResponse, "toolResponse",
            {{"stateRoot", {DataType::String}}, {"txRoot", {DataType::String}},
                {"receiptRoot", {DataType::String}}, {"logsHash", {DataType::String}},
                {"receipts", {DataType::Array}}});

        for (auto const& receipt : _toolResponse.atKey("receipts").getSubObjects())
        {
            requireJsonFields(receipt, "toolResponse_receipt",
                {{"root", {DataType::String}}, {"status", {DataType::String}},
                    {"cumulativeGasUsed", {DataType::String}}, {"logsBloom", {DataType::String}},
                    {"logs", {DataType::Null, DataType::Array}},
                    {"transactionHash", {DataType::String}},
                    {"contractAddress", {DataType::String}}, {"gasUsed", {DataType::String}},
                    {"blockHash", {DataType::String}}, {"transactionIndex", {DataType::String}}});
        }
    }
};
}  // namespace test
