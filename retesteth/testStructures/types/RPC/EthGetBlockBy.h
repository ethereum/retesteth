#pragma once
#include "../../basetypes.h"
#include "../Ethereum/BlockHeader.h"
#include "SubElements/EthGetBlockByTransaction.h"
#include <libdataobj/DataObject.h>

namespace test
{
namespace teststruct
{
// Structure for RPC response eth_getBlockByHash/eth_getBlockByNumber
struct EthGetBlockBy : GCP_SPointerBase
{
    EthGetBlockBy(spDataObject&);
    spBlockHeader const& header() const { return m_header; }
    std::vector<EthGetBlockByTransaction> const& transactions() const { return m_transactions; }
    std::vector<FH32> const& uncles() const { return m_uncles; }
    BYTES getRLPHeaderTransactions() const;

    // Check if response has a transaction
    bool hasTransaction(FH32 const& _hash) const;

private:
    EthGetBlockBy() {}
    bool m_lessobjects = false;
    spBlockHeader m_header;
    std::vector<EthGetBlockByTransaction> m_transactions;
    std::vector<FH32> m_uncles;

    spVALUE m_size;
    spVALUE m_totalDifficulty;
};

}  // namespace teststruct
}  // namespace test
