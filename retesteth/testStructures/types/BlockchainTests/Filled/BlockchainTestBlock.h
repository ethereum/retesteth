#pragma once
#include "../../../basetypes.h"
#include "../../Ethereum/BlockHeader.h"
#include "../../Ethereum/Transaction.h"
#include <retesteth/dataObject/DataObject.h>
#include <retesteth/dataObject/SPointer.h>
using namespace dataobject;

namespace test
{
namespace teststruct
{
struct BlockchainTestBlock : GCP_SPointerBase
{
    BlockchainTestBlock(DataObject const&);
    BYTES const& rlp() const { return m_rlp.getCContent(); }
    bool expectedInvalid() const { return m_blockHeader.isEmpty(); }
    spBlockHeader const& header() const { return m_blockHeader; }

    std::vector<spBlockHeader> const& uncles() const { return m_uncles; }
    std::vector<spTransaction> const& transactions() const { return m_transactions; }

private:
    BlockchainTestBlock() {}
    string m_chainName;
    spVALUE m_blockNumber;
    spBlockHeader m_blockHeader;
    std::vector<spBlockHeader> m_uncles;
    std::vector<spTransaction> m_transactions;
    spBYTES m_rlp;
};

}  // namespace teststruct
}  // namespace test
