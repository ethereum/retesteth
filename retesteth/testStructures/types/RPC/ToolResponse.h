#pragma once
#include <retesteth/testStructures/basetypes.h>
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
    VALUE const& currentExcessBlobGas() const { return m_currentExcessBlobGas; }
    VALUE const& currentBlobGasUsed() const { return m_currentBlobGasUsed; }
    FH32 const& withdrawalsRoot() const { return m_withdrawalsRoot;}
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
    void attachState(spState _state) { m_stateResponse = _state; }
    void attachDebugTrace(FH32 const& _trHash, spDebugVMTrace const& _debug) { m_debugTrace[_trHash] = _debug; }
    std::map<FH32, spDebugVMTrace> const& debugTrace() const { return m_debugTrace; }
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
    spVALUE m_currentExcessBlobGas;
    spVALUE m_currentBlobGasUsed;
    spFH32 m_withdrawalsRoot;
    std::vector<ToolResponseReceipt> m_receipts;
    spState m_stateResponse;
    std::map<FH32, spDebugVMTrace> m_debugTrace;
    std::vector<ToolResponseRejected> m_rejectedTransactions;
};

}  // namespace teststruct
}  // namespace test
