#pragma once
#include "../../basetypes.h"
#include "../Ethereum/BlockHeader.h"
#include "../Ethereum/State.h"
#include "../Ethereum/Transaction.h"


#include <EthChecks.h>
#include <dataObject/DataObject.h>
#include <dataObject/SPointer.h>
#include <libdevcore/RLP.h>
#include <testStructures/types/RPC/DebugVMTrace.h>
using namespace dataobject;
using namespace test::teststruct;

namespace test
{
namespace teststruct
{
// Ethereum Block for RLP managment
struct EthereumBlock : GCP_SPointerBase
{
    EthereumBlock(BlockHeader const& _header) { m_header = spBlockHeader(new BlockHeader(_header.asDataObject())); }
    void addTransaction(spTransaction const& _tr) { m_transactions.push_back(_tr); }
    void addUncle(BlockHeader const& _header) { m_uncles.push_back(BlockHeader(_header.asDataObject())); }
    void replaceHeader(BlockHeader const& _header) { m_header = spBlockHeader(new BlockHeader(_header.asDataObject())); }
    void recalculateUncleHash();
    BYTES const getRLP() const;

    BlockHeader const& header() const { return m_header.getCContent(); }
    BlockHeader& headerUnsafe() { return m_header.getContent(); }
    std::vector<BlockHeader> const& uncles() const { return m_uncles; }
    std::vector<spTransaction> const& transactions() const { return m_transactions; }

protected:
    EthereumBlock() {}
    spBlockHeader m_header;
    std::vector<spTransaction> m_transactions;
    std::vector<BlockHeader> m_uncles;
};

struct EthereumBlockState : EthereumBlock
{
    EthereumBlockState(BlockHeader const& _header, State const& _state, FH32 const& _logHash)
      : m_state(_state.asDataObject()), m_logHash(_logHash)
    {
        m_header = spBlockHeader(new BlockHeader(_header.asDataObject()));
    }

    void addTotalDifficulty(VALUE const& _totalDifficulty) { m_totalDifficulty = spVALUE(new VALUE(_totalDifficulty)); }
    VALUE const& totalDifficulty() const { return m_totalDifficulty.getCContent(); }

    State const& state() const { return m_state; }
    FH32 const& logHash() const { return m_logHash; }

    DebugVMTrace const& getTrTrace(FH32 const& _hash) const
    {
        if (m_transactionsTrace.count(_hash))
            return m_transactionsTrace.at(_hash);
        else
            ETH_ERROR_MESSAGE("Transaction trace not found! (" + _hash.asString() + ")");
        static DebugVMTrace empty("", "", FH32::zero(), "");
        return empty;
    }

    void setTrsTrace(std::map<FH32, DebugVMTrace> const& _map) { m_transactionsTrace = _map; }

private:
    /// EthereumBlockState(){}
    State m_state;
    FH32 m_logHash;
    spVALUE m_totalDifficulty;
    std::map<FH32, spFH32> m_transactionsLog;
    std::map<FH32, DebugVMTrace> m_transactionsTrace;
};

typedef GCP_SPointer<EthereumBlock> spEthereumBlock;
typedef GCP_SPointer<EthereumBlockState> spEthereumBlockState;

}  // namespace teststruct
}  // namespace test
