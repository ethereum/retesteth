#include "EthGetBlockBy.h"
#include "../Ethereum/EthereumBlock.h"
#include "EthGetBlockByTransaction.h"
#include <libdevcore/RLP.h>
#include <retesteth/EthChecks.h>

using namespace dev;
namespace test
{
namespace teststruct
{
EthGetBlockBy::EthGetBlockBy(DataObject const& _data)
{
    try
    {
        m_header = spBlockHeader(new BlockHeader(_data));

        m_miner = spFH20(new FH20(_data.atKey("miner")));
        m_size = spVALUE(new VALUE(_data.atKey("size")));
        m_totalDifficulty = spVALUE(new VALUE(_data.atKey("totalDifficulty")));

        m_lessobjects = false;
        for (auto const& el : _data.atKey("transactions").getSubObjects())
        {
            DataObject cel = el;
            cel.renameKey("input", "data");
            cel.renameKey("gas", "gasLimit");

            m_transactions.push_back(EthGetBlockByTransaction(cel));
            if (!m_transactions.at(m_transactions.size() - 1).isFullTransaction())
                m_lessobjects = true;
        }

        // Remote eth_getBlockBy* always return uncles as hashes.
        for (auto const& un : _data.atKey("uncles").getSubObjects())
            m_uncles.push_back(FH32(un));
    }
    catch (std::exception const& _ex)
    {
        ETH_ERROR_MESSAGE(string("EthGetBlockBy unmarshal error: ") + _ex.what());
    }
}

bool EthGetBlockBy::hasTransaction(FH32 const& _hash) const
{
    for (auto const& tr : m_transactions)
    {
        if (tr.hash() == _hash)
            return true;
    }
    return false;
}

// Because EthGetBlockBy does not return uncle headers
BYTES EthGetBlockBy::getRLPHeaderTransactions() const
{
    EthereumBlock block(m_header.getCContent());
    for (auto const& tr : m_transactions)
        block.addTransaction(tr.transaction());
    return block.getRLP();
}

}  // namespace teststruct
}  // namespace test
