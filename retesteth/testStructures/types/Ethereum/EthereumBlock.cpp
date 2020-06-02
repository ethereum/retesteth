#include "EthereumBlock.h"
#include <libdevcore/RLP.h>
#include <libdevcore/SHA3.h>
#include <retesteth/EthChecks.h>
using namespace dev;

namespace test
{
namespace teststruct
{
void EthereumBlock::recalculateHash()
{
    // Transaction List
    RLPStream transactionList(m_transactions.size());
    for (auto const& tr : m_transactions)
        transactionList.appendRaw(tr.asRLPStream().out());

    FH32 const& txHash = m_header.getContent().transactionRoot();
    FH32& txHashRef = const_cast<FH32&>(txHash);
    txHashRef = FH32("0x" + toString(dev::sha3(transactionList.out())));

    // Uncle List
    RLPStream uncleList(m_uncles.size());
    for (auto const& un : m_uncles)
        uncleList.appendRaw(un.asRLPStream().out());

    FH32 const& unHash = m_header.getContent().uncleHash();
    FH32& unHashRef = const_cast<FH32&>(unHash);
    unHashRef = FH32("0x" + toString(dev::sha3(uncleList.out())));

    // Final header hash
    FH32 const& hHash = m_header.getContent().hash();
    FH32& hHashRef = const_cast<FH32&>(hHash);
    hHashRef = FH32("0x" + toString(dev::sha3(m_header.getCContent().asRLPStream().out())));
}

BYTES const EthereumBlock::getRLP() const
{
    try
    {
        // RLP of a block
        RLPStream stream(3);
        stream.appendRaw(m_header.getCContent().asRLPStream().out());

        // Transaction list
        RLPStream transactionList(m_transactions.size());
        for (auto const& tr : m_transactions)
            transactionList.appendRaw(tr.asRLPStream().out());
        stream.appendRaw(transactionList.out());

        // Uncle list
        RLPStream uncleList(m_uncles.size());
        for (auto const& un : m_uncles)
            uncleList.appendRaw(un.asRLPStream().out());
        stream.appendRaw(uncleList.out());

        return BYTES(dev::toHexPrefixed(stream.out()));
    }
    catch (std::exception const& _ex)
    {
        ETH_ERROR_MESSAGE(string("EthereumBlock::getRLP() ") + _ex.what());
    }
    return BYTES(DataObject());
}

}  // namespace teststruct
}  // namespace test
