#include "ToolResponse.h"
#include "Constants.h"
#include <retesteth/testStructures/Common.h>
#include <retesteth/Constants.h>
#include <retesteth/EthChecks.h>
using namespace test::teststruct::constnames;

namespace test::teststruct
{
ToolResponse::ToolResponse(DataObject const& _data)
{
    REQUIRE_JSONFIELDS(_data, "ToolResponse " + _data.getKey(),
        {{c_stateRoot, {{DataType::String}, jsonField::Required}},
            {"txRoot", {{DataType::String}, jsonField::Required}},
            {c_receiptsRoot, {{DataType::String}, jsonField::Required}},
            {"logsHash", {{DataType::String}, jsonField::Required}},
            {c_logsBloom, {{DataType::String}, jsonField::Required}},
            {"currentDifficulty", {{DataType::String, DataType::Null}, jsonField::Required}},
            {"currentBaseFee", {{DataType::String, DataType::Null}, jsonField::Optional}},
            {c_withdrawalsRoot, {{DataType::String}, jsonField::Optional}},
            {"rejected", {{DataType::Array}, jsonField::Optional}},
            {c_gasUsed, {{DataType::String}, jsonField::Optional}},
            {c_currentBlobGasUsed, {{DataType::String, DataType::Null}, jsonField::Optional}},
            {c_excessBlobGas, {{DataType::String}, jsonField::Optional}},
            {c_currentExcessBlobGas, {{DataType::String}, jsonField::Optional}},
            {c_blobGasUsed, {{DataType::String, DataType::Null}, jsonField::Optional}},
            {"receipts", {{DataType::Array}, jsonField::Required}}});

    m_stateRoot = sFH32(_data.atKey(c_stateRoot));
    m_txRoot = sFH32(_data.atKey("txRoot"));
    m_receiptsRoot = sFH32(_data.atKey(c_receiptsRoot));
    m_logsHash = sFH32(_data.atKey("logsHash"));
    m_logsBloom = sFH256(_data.atKey(c_logsBloom));

    m_currentDifficulty = sVALUE(0);
    if (_data.atKey("currentDifficulty").type() != DataType::Null)
        m_currentDifficulty = sVALUE(_data.atKey("currentDifficulty"));

    m_currentBasefee = sVALUE(0);
    if (_data.count("currentBaseFee") && _data.atKey("currentBaseFee").type() != DataType::Null)
        m_currentBasefee = sVALUE(_data.atKey("currentBaseFee"));

    m_currentExcessBlobGas = sVALUE(0);
    if (_data.count(c_excessBlobGas))
        m_currentExcessBlobGas = sVALUE(_data.atKey(c_excessBlobGas));
    if (_data.count(c_currentExcessBlobGas))
        m_currentExcessBlobGas = sVALUE(_data.atKey(c_currentExcessBlobGas));
    if (_data.count(c_excessBlobGas) && _data.count(c_currentExcessBlobGas))
    {
        ETH_WARNING("Tool response inconsistent: has both `currentExcessBlobGas` and `excessBlobGas` fields!");
    }

    m_currentBlobGasUsed = sVALUE(0);
    if (_data.count(c_currentBlobGasUsed) && _data.atKey(c_currentBlobGasUsed).type() != DataType::Null)
        m_currentBlobGasUsed = sVALUE(_data.atKey(c_currentBlobGasUsed));
    if (_data.count(c_blobGasUsed) && _data.atKey(c_blobGasUsed).type() != DataType::Null)
        m_currentBlobGasUsed = sVALUE(_data.atKey(c_blobGasUsed));
    if (_data.count(c_blobGasUsed) && _data.count(c_currentBlobGasUsed))
    {
        ETH_WARNING("Tool response inconsistent: has both `currentBlobGasUsed` and `blobGasUsed` fields!");
    }

    m_withdrawalsRoot = spFH32(C_FH32_ZERO.copy());
    if (_data.count(c_withdrawalsRoot))
        m_withdrawalsRoot = sFH32(_data.atKey(c_withdrawalsRoot));

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
