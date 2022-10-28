#pragma once
#include "../../basetypes.h"
#include "../Ethereum/State.h"
#include "SubElements/ToolResponseReceipt.h"
#include "SubElements/ToolResponseRejected.h"
#include <libdataobj/DataObject.h>
#include <testStructures/types/RPC/DebugVMTrace.h>

namespace test
{
namespace teststruct
{
struct ToolResponse
{
    ToolResponse(DataObject const& _data);
    FH32 const& stateRoot() const { return m_stateRoot; }
    FH32 const& txRoot() const { return m_txRoot; }
    FH32 const& receiptRoot() const { return m_receiptsRoot; }
    FH32 const& logsHash() const { return m_logsHash; }
    FH256 const& logsBloom() const { return m_logsBloom; }
    VALUE const& currentDifficulty() const { return m_currentDifficulty; }
    VALUE const& currentBasefee() const { return m_currentBasefee; }
    VALUE totalGasUsed() const
    {
        VALUE totalGasUsed = 0;
        for (auto const& rec : m_receipts)
            totalGasUsed += rec.gasUsed();
        return totalGasUsed;
    }
    spState const& state() const { return m_stateResponse; }
    std::vector<ToolResponseReceipt> const& receipts() const { return m_receipts; }

    // Tool export the state separately
    void attachState(State const& _state) { m_stateResponse = spState(new State(_state)); }
    void attachDebugTrace(FH32 const& _trHash, DebugVMTrace const& _debug) { m_debugTrace[_trHash] = _debug; }
    std::map<FH32, DebugVMTrace> const& debugTrace() const { return m_debugTrace; }
    std::vector<ToolResponseRejected> const& rejected() const { return m_rejectedTransactions; }

private:
    ToolResponse() {}
    spFH32 m_stateRoot;
    spFH32 m_txRoot;
    spFH32 m_receiptsRoot;
    spFH32 m_logsHash;
    spFH256 m_logsBloom;
    spVALUE m_currentDifficulty;
    spVALUE m_currentBasefee;
    std::vector<ToolResponseReceipt> m_receipts;
    spState m_stateResponse;
    std::map<FH32, DebugVMTrace> m_debugTrace;

    std::vector<ToolResponseRejected> m_rejectedTransactions;
};

}  // namespace teststruct
}  // namespace test
