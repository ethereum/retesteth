#pragma once
#include "../../../basetypes.h"
#include "../../Ethereum/Transaction.h"
#include <libdataobj/DataObject.h>

namespace test
{
namespace teststruct
{
// Transaction Structure inside RPC response eth_getBlockByHash/eth_getBlockByNumber
struct EthGetBlockByTransaction
{
    EthGetBlockByTransaction(spDataObjectMove);
    FH32 const& hash() const { return m_hash; }
    spTransaction const& transaction() const
    {
        assert(isFullTransaction());
        return m_transaction;
    }
    bool isFullTransaction() const { return !m_transaction.isEmpty(); }
    FH32 const& blockHash() const
    {
        assert(isFullTransaction());
        return m_blockHash;
    }
    VALUE const& blockNumber() const
    {
        assert(isFullTransaction());
        return m_blockNumber;
    }

private:
    spTransaction m_transaction;

    spFH32 m_blockHash;
    spVALUE m_blockNumber;
    spFH20 m_from;
    spFH32 m_hash;
    spVALUE m_transactionIndex;
};

}  // namespace teststruct
}  // namespace test
