#pragma once
#include <retesteth/testStructures/basetypes.h>
#include "../Ethereum/Blocks/BlockHeader.h"
#include "SubElements/EthGetBlockByTransaction.h"
#include "SubElements/EthGetBlockByWithdrawal.h"
#include <libdataobj/DataObject.h>

namespace test::teststruct
{

// Structure for RPC response eth_getBlockByHash/eth_getBlockByNumber
struct EthGetBlockBy : GCP_SPointerBase
{
    EthGetBlockBy(spDataObject&);
    spBlockHeader const& header() const { return m_header; }
    std::vector<spEthGetBlockByTransaction> const& transactions() const { return m_transactions; }
    std::vector<FH32> const& uncles() const { return m_uncles; }
    BYTES getRLPHeaderTransactions() const;

    // Check if response has a transaction
    bool hasTransaction(FH32 const& _hash) const;

private:
    EthGetBlockBy() {}
    bool m_lessobjects = false;
    spBlockHeader m_header;
    std::vector<spEthGetBlockByTransaction> m_transactions;
    std::vector<spEthGetBlockByWithdrawal> m_withdrawals;
    std::vector<FH32> m_uncles;

    spVALUE m_size;
    spVALUE m_totalDifficulty;
};

typedef  GCP_SPointer<EthGetBlockBy> spEthGetBlockBy;

}  // namespace teststruct
