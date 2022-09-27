#pragma once
#include "../../basetypes.h"
#include "../Ethereum/BlockHeader.h"
#include "../Ethereum/BlockHeaderReader.h"
#include "../Ethereum/State.h"
#include "../Ethereum/Transaction.h"

#include <EthChecks.h>
#include <libdataobj/DataObject.h>
#include <testStructures/types/RPC/DebugVMTrace.h>

namespace test
{
namespace teststruct
{
// Ethereum Block for RLP managment
struct EthereumBlock : GCP_SPointerBase
{
    EthereumBlock(spBlockHeader const& _header) : m_header(_header) {}
    void addTransaction(spTransaction const& _tr) { m_transactions.push_back(_tr); }
    void addUncle(spBlockHeader const& _header) { m_uncles.push_back(_header); }
    void replaceHeader(spBlockHeader const& _header) { m_header = readBlockHeader(_header->asDataObject()); }
    void recalculateUncleHash();
    BYTES const getRLP() const;

    spBlockHeader const& header() const { return m_header; }
    spBlockHeader& headerUnsafe() { return m_header; }
    std::vector<spBlockHeader> const& uncles() const { return m_uncles; }
    std::vector<spTransaction> const& transactions() const { return m_transactions; }

protected:
    EthereumBlock() {}
    spBlockHeader m_header;
    std::vector<spTransaction> m_transactions;
    std::vector<spBlockHeader> m_uncles;
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
    void setTrsTrace(std::map<FH32, DebugVMTrace> const& _map) { m_transactionsTrace = _map; }

private:
    /// EthereumBlockState(){}
    const spState m_state;
    FH32 m_logHash;
    spVALUE m_totalDifficulty;
    std::map<FH32, spFH32> m_transactionsLog;
    std::map<FH32, DebugVMTrace> m_transactionsTrace;
};

typedef GCP_SPointer<EthereumBlock> spEthereumBlock;
typedef GCP_SPointer<EthereumBlockState> spEthereumBlockState;

}  // namespace teststruct
}  // namespace test
