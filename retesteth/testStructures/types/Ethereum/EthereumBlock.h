#pragma once
#include <retesteth/testStructures/basetypes.h>
#include "../Ethereum/Blocks/BlockHeader.h"
#include "../Ethereum/Blocks/BlockHeaderReader.h"
#include "../Ethereum/State.h"
#include "../Ethereum/Transactions/Transaction.h"
#include "../Ethereum/Withdrawals.h"

#include <EthChecks.h>
#include <libdataobj/DataObject.h>
#include <testStructures/types/RPC/DebugVMTrace.h>

namespace test::teststruct
{

// Ethereum Block for RLP managment
struct EthereumBlock : GCP_SPointerBase
{
    EthereumBlock(spBlockHeader const& _header) : m_header(_header) {}
    void addTransaction(spTransaction const& _tr) { m_transactions.emplace_back(_tr); }
    void addUncle(spBlockHeader const& _header) { m_uncles.emplace_back(_header); }
    void addWithdrawal(spWithdrawal const& _withdrawal) { m_withdrawals.emplace_back(_withdrawal); }
    void replaceHeader(spBlockHeader const& _header) { m_header = readBlockHeader(_header->asDataObject()); }
    void recalculateUncleHash();
    BYTES const getRLP() const;
    void forceWithdrawalsRLP() { m_forceWithdrawalsRLP = true; }
    void forceNoWithdrawalsRLP() { m_forceNoWithdrawalsRLP = true; }

    spBlockHeader const& header() const { return m_header; }
    spBlockHeader& headerUnsafe() { return m_header; }
    std::vector<spBlockHeader> const& uncles() const { return m_uncles; }
    std::vector<spTransaction> const& transactions() const { return m_transactions; }
    bool hasTransaction(FH32 const&) const;
    void clear() { m_transactions.clear(); m_withdrawals.clear(); m_uncles.clear(); }
    std::vector<spWithdrawal> const& withdrawals() const { return m_withdrawals; }

protected:
    EthereumBlock() {}
    spBlockHeader m_header;
    std::vector<spTransaction> m_transactions;
    std::vector<spBlockHeader> m_uncles;
    std::vector<spWithdrawal> m_withdrawals;
protected:
    bool m_forceWithdrawalsRLP = false;
    bool m_forceNoWithdrawalsRLP = false;
};

struct EthereumBlockState : EthereumBlock
{
    EthereumBlockState(spBlockHeader const& _header, spState const& _state, FH32 const& _logHash)
      : m_state(_state), m_logHash(_logHash.asString())
    {
        m_header = readBlockHeader(_header->asDataObject());
        m_totalDifficulty = spVALUE(new VALUE(_header->difficulty().asBigInt()));
    }

    void setTotalDifficulty(VALUE const& _totalDifficulty) { m_totalDifficulty = spVALUE(_totalDifficulty.copy()); }
    VALUE const& totalDifficulty() const { return m_totalDifficulty; }

    spState const& state() const { return m_state; }
    FH32 const& logHash() const { return m_logHash; }

    // Debug
    DebugVMTrace const& getTrTrace(FH32 const& _hash) const;
    void setTrsTrace(std::map<FH32, spDebugVMTrace> const& _map) { m_transactionsTrace = _map; }

private:
    /// EthereumBlockState(){}
    const spState m_state;
    FH32 m_logHash;
    spVALUE m_totalDifficulty;
    std::map<FH32, spFH32> m_transactionsLog;
    std::map<FH32, spDebugVMTrace> m_transactionsTrace;
};

typedef GCP_SPointer<EthereumBlock> spEthereumBlock;
typedef GCP_SPointer<EthereumBlockState> spEthereumBlockState;

}  // namespace teststruct
