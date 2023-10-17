#include "ToolResponseReceipt.h"
#include <retesteth/testStructures/Common.h>
#include <retesteth/Constants.h>
using namespace test::teststruct::constnames;

namespace test::teststruct
{
ToolResponseReceipt::ToolResponseReceipt(DataObject const& _data)
{
    REQUIRE_JSONFIELDS(_data, "ToolResponseReceipt " + _data.getKey(),
        {{"type", {{DataType::String}, jsonField::Optional}},
            {"root", {{DataType::String}, jsonField::Optional}},
            {"status", {{DataType::String}, jsonField::Optional}},
            {"cumulativeGasUsed", {{DataType::String}, jsonField::Optional}},
            {"logsBloom", {{DataType::String}, jsonField::Optional}},
            {"logs", {{DataType::Null, DataType::Array}, jsonField::Optional}},
            {c_transactionHash, {{DataType::String}, jsonField::Required}},
            {"contractAddress", {{DataType::String}, jsonField::Optional}},
            {c_gasUsed, {{DataType::String}, jsonField::Required}},
            {"effectiveGasPrice", {{DataType::Null}, jsonField::Optional}},
            {c_blockHash, {{DataType::String}, jsonField::Optional}},
            {"transactionIndex", {{DataType::String}, jsonField::Optional}}});

    m_trHash = sFH32(_data.atKey(c_transactionHash));
    m_trGasUsed = sVALUE(_data.atKey(c_gasUsed));
}


}  // namespace teststruct
