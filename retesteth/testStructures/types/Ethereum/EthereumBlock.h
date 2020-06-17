#pragma once
#include "../../basetypes.h"
#include "../Ethereum/BlockHeader.h"
#include "../Ethereum/State.h"
#include "../Ethereum/Transaction.h"


#include <libdevcore/RLP.h>
#include <retesteth/dataObject/DataObject.h>
#include <retesteth/dataObject/SPointer.h>
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
    void addTransaction(Transaction const& _tr) { m_transactions.push_back(Transaction(_tr.asDataObject())); }
    void addUncle(BlockHeader const& _header) { m_uncles.push_back(BlockHeader(_header.asDataObject())); }
    void replaceHeader(BlockHeader const& _header) { m_header = spBlockHeader(new BlockHeader(_header.asDataObject())); }
    void recalculateUncleHash();
    BYTES const getRLP() const;

    BlockHeader const& header() const { return m_header.getCContent(); }
    BlockHeader& headerUnsafe() { return m_header.getContent(); }
    std::vector<BlockHeader> const& uncles() const { return m_uncles; }
    std::vector<Transaction> const& transactions() const { return m_transactions; }

protected:
    EthereumBlock() {}
    spBlockHeader m_header;
    std::vector<Transaction> m_transactions;
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

private:
    /// EthereumBlockState(){}
    State m_state;
    FH32 m_logHash;
    spVALUE m_totalDifficulty;
    std::map<FH32, spFH32> m_transactionsLog;
};

typedef GCP_SPointer<EthereumBlock> spEthereumBlock;
typedef GCP_SPointer<EthereumBlockState> spEthereumBlockState;

}  // namespace teststruct
}  // namespace test
