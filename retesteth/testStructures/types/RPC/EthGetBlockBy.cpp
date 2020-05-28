#include "EthGetBlockBy.h"
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

BYTES const EthGetBlockBy::getRLP() const
{
    ETH_ERROR_REQUIRE_MESSAGE(false, "EthGetBlockBy unable to construct RLP because remote does not provide uncles");
    ETH_ERROR_REQUIRE_MESSAGE(!m_lessobjects, "EthGetBlockBy::getRLP() of a block received without full transactions!");
    try
    {
        // RLP of a block
        RLPStream stream(3);
        stream.appendRaw(m_header.getCContent().asRLPStream().out());

        // Transaction list
        RLPStream transactionList(m_transactions.size());
        for (auto const& tr : m_transactions)
            transactionList.appendRaw(tr.transaction().asRLPStream().out());
        stream.appendRaw(transactionList.out());

        // Uncle list
        RLPStream uncleList(0);  // Unable to construct uncle list
                                 //        RLPStream uncleList(m_uncles.size());
                                 //        for (auto const& un : m_uncles)
                                 //            uncleList.appendRaw(un.asRLPStream().out());
                                 //        stream.appendRaw(uncleList.out());
        return BYTES(dev::toHexPrefixed(stream.out()));
    }
    catch (std::exception const& _ex)
    {
        ETH_ERROR_MESSAGE(string("EthGetBlockBy::getRLP() ") + _ex.what());
    }
    return BYTES(DataObject());
}

// Return fake RLP always setting transactions and uncles as empty
BYTES const EthGetBlockBy::fakeRLP() const
{
    try
    {
        RLPStream stream(3);
        stream.appendRaw(m_header.getCContent().asRLPStream().out());
        RLPStream transactionList(0);
        stream.appendRaw(transactionList.out());
        RLPStream uncleList(0);
        stream.appendRaw(uncleList.out());
        return BYTES(dev::toHexPrefixed(stream.out()));
    }
    catch (std::exception const& _ex)
    {
        ETH_ERROR_MESSAGE(string("EthGetBlockBy::fakeRLP() ") + _ex.what());
    }
    return BYTES(DataObject());
}


}  // namespace teststruct
}  // namespace test
