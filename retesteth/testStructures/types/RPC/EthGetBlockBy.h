#pragma once
#include "../../basetypes.h"
#include "../Ethereum/BlockHeader.h"
#include "SubElements/EthGetBlockByTransaction.h"
#include <retesteth/dataObject/DataObject.h>

using namespace dataobject;

namespace test
{
namespace teststruct
{
// Structure for RPC response eth_getBlockByHash/eth_getBlockByNumber
struct EthGetBlockBy : GCP_SPointerBase
{
    EthGetBlockBy(DataObject const&);
    spBlockHeader const& header() const { return m_header; }
    std::vector<EthGetBlockByTransaction> const& transactions() const { return m_transactions; }
    std::vector<FH32> const& uncles() const { return m_uncles; }
    BYTES getRLPHeaderTransactions() const;

    // Check if response has a transaction
    bool hasTransaction(FH32 const& _hash) const;
    std::string const& getTrException(FH32 const& _hash) const
    {
        static std::string empty;
        if (m_rejectedTransactions.count(_hash))
            return m_rejectedTransactions.at(_hash);
        return empty;
    }

private:
    EthGetBlockBy() {}
    bool m_lessobjects = false;
    spBlockHeader m_header;
    std::vector<EthGetBlockByTransaction> m_transactions;
    std::vector<FH32> m_uncles;
    std::map<FH32, string> m_rejectedTransactions;

    spVALUE m_size;
    spVALUE m_totalDifficulty;
};

}  // namespace teststruct
}  // namespace test
