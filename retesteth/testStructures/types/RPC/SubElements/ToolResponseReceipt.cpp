#include "ToolResponseReceipt.h"
#include <retesteth/testStructures/Common.h>

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
            {"blockHash", {{DataType::String}, jsonField::Required}},
            {"transactionIndex", {{DataType::String}, jsonField::Required}}});

    m_trHash = spFH32(new FH32(_data.atKey("transactionHash")));
    m_blockHash = spFH32(new FH32(_data.atKey("blockHash")));
    m_trGasUsed = spVALUE(new VALUE(_data.atKey("gasUsed")));
}


}  // namespace teststruct
