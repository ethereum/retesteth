#include "BlockchainTestFillerEnv.h"
#include <retesteth/EthChecks.h>
#include <retesteth/testStructures/Common.h>
namespace test
{
namespace teststruct
{
// Env. Build a type representation
BlockchainTestFillerEnv::BlockchainTestFillerEnv(DataObject const& _data)
{
    try
    {
        DataObject tmpData = _data;
        tmpData.performModifier(mod_valueToCompactEvenHexPrefixed);

        DataObject coinbase = _data.atKey("coinbase");
        if (coinbase.asString().size() > 1 && coinbase.asString()[1] != 'x')
            coinbase = "0x" + coinbase.asString();
        m_currentCoinbase = spFH20(new FH20(coinbase));

        m_currentDifficulty = spVALUE(new VALUE(tmpData.atKey("difficulty")));
        m_currentGasLimit = spVALUE(new VALUE(tmpData.atKey("gasLimit"), dev::u256("0x7fffffffffffffff")));
        m_currentNumber = spVALUE(new VALUE(tmpData.atKey("number")));
        m_currentTimestamp = spVALUE(new VALUE(tmpData.atKey("timestamp")));
        m_previousHash = spFH32(new FH32(_data.atKey("parentHash")));

        m_currentExtraData = spBYTES(new BYTES(_data.atKey("extraData")));
        m_currentNonce = spFH8(new FH8(FH8::zero()));       // spFH8(new FH8(_data.atKey("nonce")));
        m_currentMixHash = spFH32(new FH32(FH32::zero()));  // spFH32(new FH32(_data.atKey("mixHash")));

        // Allowed fields for this structure
        requireJsonFields(_data, "GenesisBlockHeader(BlockchainTestFillerEnv) " + _data.getKey(),
            {{"bloom", {{DataType::String}, jsonField::Optional}},
             {"logsBloom", {{DataType::String}, jsonField::Optional}},
             {"coinbase", {{DataType::String}, jsonField::Optional}},
             {"author", {{DataType::String}, jsonField::Optional}},
             {"miner", {{DataType::String}, jsonField::Optional}},
             {"difficulty", {{DataType::String}, jsonField::Required}},
             {"extraData", {{DataType::String}, jsonField::Required}},
             {"gasLimit", {{DataType::String}, jsonField::Required}},
             {"gasUsed", {{DataType::String}, jsonField::Required}},
             {"hash", {{DataType::String}, jsonField::Optional}},
             {"mixHash", {{DataType::String}, jsonField::Optional}},
             {"nonce", {{DataType::String}, jsonField::Optional}},
             {"number", {{DataType::String}, jsonField::Required}},
             {"parentHash", {{DataType::String}, jsonField::Required}},
             {"receiptTrie", {{DataType::String}, jsonField::Optional}},
             {"receiptsRoot", {{DataType::String}, jsonField::Optional}},
             {"stateRoot", {{DataType::String}, jsonField::Required}},
             {"timestamp", {{DataType::String}, jsonField::Required}},
             {"transactionsTrie", {{DataType::String}, jsonField::Optional}},
             {"transactionsRoot", {{DataType::String}, jsonField::Optional}},
             {"sha3Uncles", {{DataType::String}, jsonField::Optional}},
             {"uncleHash", {{DataType::String}, jsonField::Optional}}
                          });

    }
    catch (std::exception const& _ex)
    {
        throw BaseEthException(string("BlockchainTestFillerEnv convertion error: ") + _ex.what());
    }
}

}  // namespace teststruct
}  // namespace test
