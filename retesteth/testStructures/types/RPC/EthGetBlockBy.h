#pragma once
#include "../../basetypes.h"
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
    FH32 const& stateRoot() const { return m_stateRoot.getCContent(); }
    std::vector<EthGetBlockByTransaction> const& transactions() const { return m_transactions; }

    // Check if response has a transaction
    bool hasTransaction(FH32 const& _hash) const;

private:
    std::vector<EthGetBlockByTransaction> m_transactions;
    spFH20 m_author;
    spVALUE m_difficulty;
    spBYTES m_extraData;
    spVALUE m_gasLimit;
    spVALUE m_gasUsed;
    spFH32 m_hash;
    spFH256 m_logsBloom;
    spFH20 m_miner;
    spFH32 m_mixHash;
    spFH8 m_nonce;
    spVALUE m_number;
    spFH32 m_parentHash;
    spFH32 m_receiptsRoot;
    spFH32 m_sha3Uncles;
    spVALUE m_size;
    spFH32 m_stateRoot;
    spVALUE m_timestamp;
    spVALUE m_totalDifficulty;
    spFH32 m_transactionsRoot;
};

}  // namespace teststruct
}  // namespace test
