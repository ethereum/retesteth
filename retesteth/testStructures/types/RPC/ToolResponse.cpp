#include "ToolResponse.h"
#include "Constants.h"
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
            {"withdrawalsRoot", {{DataType::String}, jsonField::Optional}},
            {"rejected", {{DataType::Array}, jsonField::Optional}},
            {"gasUsed", {{DataType::String}, jsonField::Optional}},
            {"receipts", {{DataType::Array}, jsonField::Required}}});

    m_stateRoot = spFH32(new FH32(_data.atKey("stateRoot")));
    m_txRoot = spFH32(new FH32(_data.atKey("txRoot")));
    m_receiptsRoot = spFH32(new FH32(_data.atKey("receiptsRoot")));
    m_logsHash = spFH32(new FH32(_data.atKey("logsHash")));
    m_logsBloom = spFH256(new FH256(_data.atKey("logsBloom")));

    m_currentDifficulty = spVALUE(new VALUE(0));
    if (_data.atKey("currentDifficulty").type() != DataType::Null)
        m_currentDifficulty = spVALUE(new VALUE(_data.atKey("currentDifficulty")));

    m_currentBasefee = spVALUE(new VALUE(0));
    if (_data.count("currentBaseFee"))
        m_currentBasefee = spVALUE(new VALUE(_data.atKey("currentBaseFee")));

    m_withdrawalsRoot = spFH32(new FH32(C_WITHDRAWALS_EMPTY_ROOT));
    if (_data.count("withdrawalsRoot"))
        m_withdrawalsRoot = spFH32(new FH32(_data.atKey("withdrawalsRoot")));

    auto const& receipts = _data.atKey("receipts").getSubObjects();
    m_receipts.reserve(receipts.size());
    for (auto const& el : receipts)
        m_receipts.emplace_back(ToolResponseReceipt(el));

    if (_data.count("rejected"))
    {
        for (auto const& el : _data.atKey("rejected").getSubObjects())
            m_rejectedTransactions.emplace_back(ToolResponseRejected(el));
    }
}


}  // namespace teststruct
