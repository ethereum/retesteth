#include "EthGetBlockBy.h"
#include "../Ethereum/EthereumBlock.h"
#include "SubElements/EthGetBlockByTransaction.h"
#include <libdevcore/RLP.h>
#include <retesteth/EthChecks.h>
#include <retesteth/testStructures/Common.h>

using namespace dev;

namespace test
{
namespace teststruct
{
EthGetBlockBy::EthGetBlockBy(DataObject const& _data)
{
    try
    {
        m_header = readBlockHeader(_data); // BlockHeader verify _data fields

        m_size = spVALUE(new VALUE(_data.atKey("size")));
        m_totalDifficulty = spVALUE(new VALUE(_data.atKey("totalDifficulty")));

        m_lessobjects = false;
        for (auto const& el : _data.atKey("transactions").getSubObjects())
        {
            spDataObject cel;
            (*cel).copyFrom(el);
            (*cel).renameKey("input", "data");
            (*cel).renameKey("gas", "gasLimit");

            m_transactions.push_back(EthGetBlockByTransaction(cel));
            if (!m_transactions.at(m_transactions.size() - 1).isFullTransaction())
                m_lessobjects = true;
        }

        // Remote eth_getBlockBy* always return uncles as hashes.
        for (auto const& un : _data.atKey("uncles").getSubObjects())
            m_uncles.push_back(FH32(un));

        requireJsonFields(_data, "EthGetBlockBy " + _data.getKey(),
            {{"logsBloom", {{DataType::String}, jsonField::Required}},
             {"author", {{DataType::String}, jsonField::Optional}},                 //Geth return field
             {"miner", {{DataType::String}, jsonField::Required}},
             {"difficulty", {{DataType::String}, jsonField::Required}},
             {"extraData", {{DataType::String}, jsonField::Required}},
             {"gasLimit", {{DataType::String}, jsonField::Optional}},
             {"baseFeePerGas", {{DataType::String}, jsonField::Optional}},
             {"gasUsed", {{DataType::String}, jsonField::Required}},
             {"hash", {{DataType::String}, jsonField::Required}},
             {"mixHash", {{DataType::String}, jsonField::Optional}},
             {"nonce", {{DataType::String}, jsonField::Optional}},
             {"number", {{DataType::String}, jsonField::Required}},
             {"parentHash", {{DataType::String}, jsonField::Required}},
             {"receiptsRoot", {{DataType::String}, jsonField::Required}},
             {"stateRoot", {{DataType::String}, jsonField::Required}},
             {"timestamp", {{DataType::String}, jsonField::Required}},
             {"transactionsRoot", {{DataType::String}, jsonField::Required}},
             {"sha3Uncles", {{DataType::String}, jsonField::Required}},
             {"seedHash", {{DataType::String}, jsonField::Optional}},               //Aleth field
             {"boundary", {{DataType::String}, jsonField::Optional}},               //Aleth field
             {"miner", {{DataType::String}, jsonField::Required}},
             {"size", {{DataType::String}, jsonField::Required}},
             {"totalDifficulty", {{DataType::String}, jsonField::Required}},
             {"uncles", {{DataType::Array}, jsonField::Required}},
             {"transactions", {{DataType::Array}, jsonField::Required}}});
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
    EthereumBlock block(m_header);
    for (auto const& tr : m_transactions)
        block.addTransaction(tr.transaction());
    return block.getRLP();
}

}  // namespace teststruct
}  // namespace test
