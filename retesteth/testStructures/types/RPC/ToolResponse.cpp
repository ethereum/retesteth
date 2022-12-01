#include "ToolResponse.h"
#include <retesteth/testStructures/Common.h>

namespace test::teststruct
{
ToolResponse::ToolResponse(DataObject const& _data)
{
    REQUIRE_JSONFIELDS(_data, "ToolResponse " + _data.getKey(),
        {{"stateRoot", {{DataType::String}, jsonField::Required}},
            {"txRoot", {{DataType::String}, jsonField::Required}},
            {"receiptsRoot", {{DataType::String}, jsonField::Required}},
            {"logsHash", {{DataType::String}, jsonField::Required}},
            {"logsBloom", {{DataType::String}, jsonField::Required}},
            {"currentDifficulty", {{DataType::String, DataType::Null}, jsonField::Required}},
            {"currentBaseFee", {{DataType::String, DataType::Null}, jsonField::Optional}},
            {"rejected", {{DataType::Array}, jsonField::Optional}},
            {"gasUsed", {{DataType::String}, jsonField::Optional}},
            {"receipts", {{DataType::Array}, jsonField::Required}}});

    m_stateRoot = spFH32(new FH32(_data.atKey("stateRoot")));
    m_txRoot = spFH32(new FH32(_data.atKey("txRoot")));
    m_receiptsRoot = spFH32(new FH32(_data.atKey("receiptsRoot")));
    m_logsHash = spFH32(new FH32(_data.atKey("logsHash")));
    m_logsBloom = spFH256(new FH256(_data.atKey("logsBloom")));

    if (_data.atKey("currentDifficulty").type() != DataType::Null)
        m_currentDifficulty = spVALUE(new VALUE(_data.atKey("currentDifficulty")));
    else
        m_currentDifficulty = spVALUE(new VALUE(0));

    if (_data.count("currentBaseFee"))
        m_currentBasefee = spVALUE(new VALUE(_data.atKey("currentBaseFee")));
    else
        m_currentBasefee = spVALUE(new VALUE(0));

    for (auto const& el : _data.atKey("receipts").getSubObjects())
        m_receipts.push_back(ToolResponseReceipt(el));

    if (_data.count("rejected"))
    {
        for (auto const& el : _data.atKey("rejected").getSubObjects())
            m_rejectedTransactions.push_back(ToolResponseRejected(el));
    }
}


}  // namespace teststruct
