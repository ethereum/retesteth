#include "ToolResponse.h"
#include <retesteth/EthChecks.h>
#include <retesteth/testStructures/Common.h>

namespace test
{
namespace teststruct
{
ToolResponse::ToolResponse(DataObject const& _data)
{
    m_stateRoot = spFH32(new FH32(_data.atKey("stateRoot")));
    m_txRoot = spFH32(new FH32(_data.atKey("txRoot")));
    m_receiptRoot = spFH32(new FH32(_data.atKey("receiptRoot")));
    m_logsHash = spFH32(new FH32(_data.atKey("logsHash")));
    // receipts array

    requireJsonFields(_data, "ToolResponse " + _data.getKey(),
        {{"stateRoot", {{DataType::String}, jsonField::Required}}, {"txRoot", {{DataType::String}, jsonField::Required}},
            {"receiptRoot", {{DataType::String}, jsonField::Required}}, {"logsHash", {{DataType::String}, jsonField::Required}},
            {"receipts", {{DataType::Array}, jsonField::Required}}});
}


}  // namespace teststruct
}  // namespace test
