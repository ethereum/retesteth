#pragma once
#include "../../basetypes.h"
#include "../Ethereum/BlockHeader.h"
#include "EthGetBlockByTransaction.h"
#include <retesteth/dataObject/DataObject.h>

using namespace dataobject;

namespace test
{
namespace teststruct
{
// Structure for RPC response eth_getBlockByHash/eth_getBlockByNumber
struct EthGetBlockBy
{
    EthGetBlockBy(DataObject const&);
    std::vector<EthGetBlockByTransaction> const& transactions() const { return m_transactions; }
    std::vector<FH32> const& uncles() const { return m_uncles; }

    BlockHeader const& header() const { return m_header.getCContent(); }
    BYTES const getRLP() const;

    // Return fake RLP always setting transactions and uncles as empty
    BYTES const fakeRLP() const;

    // Check if response has a transaction
    bool hasTransaction(FH32 const& _hash) const;

private:
    bool m_lessobjects = false;
    spBlockHeader m_header;
    std::vector<EthGetBlockByTransaction> m_transactions;
    std::vector<FH32> m_uncles;

    spFH20 m_miner;
    spVALUE m_size;
    spVALUE m_totalDifficulty;
};

}  // namespace teststruct
}  // namespace test
