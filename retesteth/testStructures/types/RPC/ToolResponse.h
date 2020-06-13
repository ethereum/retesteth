#pragma once
#include "../../basetypes.h"
#include "../Ethereum/State.h"
#include "SubElements/ToolResponseReceipt.h"
#include <retesteth/dataObject/DataObject.h>

using namespace dataobject;

namespace test
{
namespace teststruct
{
struct ToolResponse
{
    ToolResponse(DataObject const& _data);
    FH32 const& stateRoot() const { return m_stateRoot.getCContent(); }
    FH32 const& txRoot() const { return m_txRoot.getCContent(); }
    FH32 const& receiptRoot() const { return m_receiptRoot.getCContent(); }
    FH32 const& logsHash() const { return m_logsHash.getCContent(); }
    VALUE totalGasUsed() const
    {
        VALUE totalGasUsed = 0;
        for (auto const& rec : m_receipts)
            totalGasUsed = totalGasUsed + rec.gasUsed();
        return totalGasUsed;
    }
    State const& state() const { return m_stateResponse.getCContent(); }
    std::vector<ToolResponseReceipt> const& receipts() const { return m_receipts; }

    // Tool export the state separately
    void attachState(State const& _state) { m_stateResponse = spState(new State(_state)); }

private:
    ToolResponse() {}
    spFH32 m_stateRoot;
    spFH32 m_txRoot;
    spFH32 m_receiptRoot;
    spFH32 m_logsHash;
    std::vector<ToolResponseReceipt> m_receipts;
    spState m_stateResponse;
};

}  // namespace teststruct
}  // namespace test
