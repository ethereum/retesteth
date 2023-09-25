#pragma once
#include "../../../basetypes.h"
#include "../../Ethereum/Blocks/BlockHeader.h"
#include "../../Ethereum/Transactions/Transaction.h"
#include <retesteth/testStructures/types/Ethereum/Withdrawals.h>
#include <libdataobj/DataObject.h>

namespace test::teststruct
{
using namespace dataobject;
typedef std::tuple<spTransaction, std::string> TransactionException;

struct BlockchainTestBlock : GCP_SPointerBase
{
    BlockchainTestBlock(spDataObject&);
    BYTES const& rlp() const { return m_rlp; }
    bool expectedInvalid() const { return m_hasRlpDecoded || m_blockHeader.isEmpty(); }
    bool hasRlpDecoded() const { return m_hasRlpDecoded; }
    spBlockHeader const& header() const { return m_blockHeader; }
    std::string const& getExpectException() const { return m_exception; }

    std::vector<spBlockHeader> const& uncles() const { return m_uncles; }
    std::vector<spTransaction> const& transactions() const { return m_transactions; }
    std::vector<spWithdrawal> const& withdrawals() const { return m_withdrawals; }
    std::vector<TransactionException> const& transactionSequence() const { return m_transactionSequence; }

private:
    BlockchainTestBlock() {}
    bool m_hasRlpDecoded = false;
    std::string m_chainName;
    std::string m_exception;
    spVALUE m_blockNumber;
    spBlockHeader m_blockHeader;
    std::vector<spBlockHeader> m_uncles;
    std::vector<spTransaction> m_transactions;
    std::vector<spWithdrawal> m_withdrawals;
    std::vector<TransactionException> m_transactionSequence;
    spBYTES m_rlp;
};

}  // namespace teststruct
