#include "ToolResponseReceipt.h"
#include <retesteth/EthChecks.h>
#include <retesteth/testStructures/Common.h>

namespace test
{
namespace teststruct
{
ToolResponseReceipt::ToolResponseReceipt(DataObject const& _data)
{
    m_trHash = spFH32(new FH32(_data.atKey("transactionHash")));

    requireJsonFields(_data, "ToolResponseReceipt " + _data.getKey(),
        {{"root", {{DataType::String}, jsonField::Required}},
         {"status", {{DataType::String}, jsonField::Required}},
         {"cumulativeGasUsed", {{DataType::String}, jsonField::Required}},
         {"logsBloom", {{DataType::String}, jsonField::Required}},
         {"logs", {{DataType::Null, DataType::Array}, jsonField::Required}},
         {"transactionHash", {{DataType::String}, jsonField::Required}},
         {"contractAddress", {{DataType::String}, jsonField::Required}},
         {"gasUsed", {{DataType::String}, jsonField::Required}},
         {"blockHash", {{DataType::String}, jsonField::Required}},
         {"transactionIndex", {{DataType::String}, jsonField::Required}}});
}


}  // namespace teststruct
}  // namespace test
