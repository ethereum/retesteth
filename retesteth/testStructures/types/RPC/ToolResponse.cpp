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
    m_logsBloom = spFH256(new FH256(_data.atKey("logsBloom")));
    m_currentDifficulty = spVALUE(new VALUE(_data.atKey("currentDifficulty")));
    for (auto const& el : _data.atKey("receipts").getSubObjects())
        m_receipts.push_back(ToolResponseReceipt(el));

    if (_data.count("rejected"))
    {
        for (auto const& el : _data.atKey("rejected").getSubObjects())
            m_rejectedTransactions.push_back(ToolResponseRejected(el));
    }

    requireJsonFields(_data, "ToolResponse " + _data.getKey(),
        {{"stateRoot", {{DataType::String}, jsonField::Required}},
         {"txRoot", {{DataType::String}, jsonField::Required}},
         {"receiptRoot", {{DataType::String}, jsonField::Required}},
         {"logsHash", {{DataType::String}, jsonField::Required}},
         {"logsBloom", {{DataType::String}, jsonField::Required}},
         {"currentDifficulty", {{DataType::Integer}, jsonField::Required}},
         {"rejected", {{DataType::Array}, jsonField::Optional}},
         {"receipts", {{DataType::Array}, jsonField::Required}}});
}


}  // namespace teststruct
}  // namespace test
