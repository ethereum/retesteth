#pragma once
#include "../../../basetypes.h"
#include "../../Ethereum/BlockHeader.h"
#include "../../Ethereum/Transaction.h"
#include <libdataobj/DataObject.h>

namespace test::teststruct
{
using namespace dataobject;
typedef std::tuple<spTransaction, std::string> TransactionException;

struct BlockchainTestBlock : GCP_SPointerBase
{
    BlockchainTestBlock(spDataObject&);
    BYTES const& rlp() const { return m_rlp; }
    bool expectedInvalid() const { return m_blockHeader.isEmpty(); }
    spBlockHeader const& header() const { return m_blockHeader; }
    std::string const& getExpectException() const { return m_exception; }

    std::vector<spBlockHeader> const& uncles() const { return m_uncles; }
    std::vector<spTransaction> const& transactions() const { return m_transactions; }
    std::vector<TransactionException> const& transactionSequence() const { return m_transactionSequence; }

private:
    BlockchainTestBlock() {}
    std::string m_chainName;
    std::string m_exception;
    spVALUE m_blockNumber;
    spBlockHeader m_blockHeader;
    std::vector<spBlockHeader> m_uncles;
    std::vector<spTransaction> m_transactions;
    std::vector<TransactionException> m_transactionSequence;
    spBYTES m_rlp;
};

}  // namespace teststruct
