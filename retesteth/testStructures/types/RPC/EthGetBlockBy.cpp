#include "EthGetBlockBy.h"
#include "EthGetBlockByTransaction.h"
#include <retesteth/EthChecks.h>

namespace test
{
namespace teststruct
{
EthGetBlockBy::EthGetBlockBy(DataObject const& _data)
{
    try
    {
        m_author = spFH20(new FH20(_data.atKey("author")));
        m_difficulty = spVALUE(new VALUE(_data.atKey("difficulty")));
        m_extraData = spBYTES(new BYTES(_data.atKey("extraData")));
        m_gasLimit = spVALUE(new VALUE(_data.atKey("gasLimit")));
        m_gasUsed = spVALUE(new VALUE(_data.atKey("gasUsed")));
        m_hash = spFH32(new FH32(_data.atKey("hash")));
        m_logsBloom = spFH256(new FH256(_data.atKey("logsBloom")));
        m_miner = spFH20(new FH20(_data.atKey("miner")));

        if (_data.count("mixHash"))
        {
            m_mixHash = spFH32(new FH32(_data.atKey("mixHash")));
            m_nonce = spFH8(new FH8(_data.atKey("nonce")));
        }
        else
        {
            ETH_WARNING_TEST("EthGetBlockBy* `mixHash` is not defined. Using default `0x00..00` value!", 6);
            m_mixHash = spFH32(new FH32(FH32::zero()));
            m_nonce = spFH8(new FH8(FH8::zero()));
        }

        m_number = spVALUE(new VALUE(_data.atKey("number")));
        m_parentHash = spFH32(new FH32(_data.atKey("parentHash")));
        m_receiptsRoot = spFH32(new FH32(_data.atKey("receiptsRoot")));
        m_sha3Uncles = spFH32(new FH32(_data.atKey("sha3Uncles")));
        m_size = spVALUE(new VALUE(_data.atKey("size")));
        m_stateRoot = spFH32(new FH32(_data.atKey("stateRoot")));
        m_timestamp = spVALUE(new VALUE(_data.atKey("timestamp")));
        m_totalDifficulty = spVALUE(new VALUE(_data.atKey("totalDifficulty")));
        m_transactionsRoot = spFH32(new FH32(_data.atKey("transactionsRoot")));
        for (auto const& el : _data.atKey("transactions").getSubObjects())
            m_transactions.push_back(EthGetBlockByTransaction(el));
    }
    catch (std::exception const& _ex)
    {
        ETH_ERROR_MESSAGE(string("EthGetBlockBy unmarshal error: ") + _ex.what());
    }


    /*"transactions" : [
    ],
    "uncles" : [
    ]*/
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

}  // namespace teststruct
}  // namespace test
