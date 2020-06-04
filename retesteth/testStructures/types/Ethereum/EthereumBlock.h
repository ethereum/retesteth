#pragma once
#include "../../basetypes.h"
#include "../Ethereum/BlockHeader.h"
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
    void recalculateHash();
    BYTES const getRLP() const;

    BlockHeader const& header() const { return m_header.getCContent(); }
    BlockHeader& headerUnsafe() { return m_header.getContent(); }
    std::vector<BlockHeader> const& uncles() const { return m_uncles; }
    std::vector<Transaction> const& transactions() const { return m_transactions; }

private:
    EthereumBlock() {}
    spBlockHeader m_header;
    std::vector<Transaction> m_transactions;
    std::vector<BlockHeader> m_uncles;
};

typedef GCP_SPointer<EthereumBlock> spEthereumBlock;

}  // namespace teststruct
}  // namespace test
