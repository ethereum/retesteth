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
            {"root", {{DataType::String}, jsonField::Required}},
            {"status", {{DataType::String}, jsonField::Required}},
            {"cumulativeGasUsed", {{DataType::String}, jsonField::Required}},
            {"logsBloom", {{DataType::String}, jsonField::Required}},
            {"logs", {{DataType::Null, DataType::Array}, jsonField::Required}},
            {"transactionHash", {{DataType::String}, jsonField::Required}},
            {"contractAddress", {{DataType::String}, jsonField::Required}},
            {"gasUsed", {{DataType::String}, jsonField::Required}},
            {"effectiveGasPrice", {{DataType::Null}, jsonField::Optional}},
            {"blockHash", {{DataType::String}, jsonField::Required}},
            {"transactionIndex", {{DataType::String}, jsonField::Required}}});

    m_trHash = sFH32(_data.atKey(c_transactionHash));
    m_blockHash = sFH32(_data.atKey(c_blockHash));
    m_trGasUsed = sVALUE(_data.atKey(c_gasUsed));
}


}  // namespace teststruct
