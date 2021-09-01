#include "BlockchainTestFillerEnv.h"
#include <retesteth/EthChecks.h>
#include <retesteth/testStructures/Common.h>
namespace test
{
namespace teststruct
{
// Env. Build a type representation
BlockchainTestFillerEnv::BlockchainTestFillerEnv(spDataObjectMove _data, SealEngine _sEngine)
{
    try
    {
        m_raw = _data.getPointer();

        (*m_raw).atKeyUnsafe("coinbase").performModifier(mod_valueInsertZeroXPrefix);
        (*m_raw).atKeyUnsafe("difficulty").performModifier(mod_valueToCompactEvenHexPrefixed);
        (*m_raw).atKeyUnsafe("gasLimit").performModifier(mod_valueToCompactEvenHexPrefixed);
        (*m_raw).atKeyUnsafe("number").performModifier(mod_valueToCompactEvenHexPrefixed);
        (*m_raw).atKeyUnsafe("timestamp").performModifier(mod_valueToCompactEvenHexPrefixed);
        (*m_raw).atKeyUnsafe("parentHash").performModifier(mod_valueInsertZeroXPrefix);

        (*m_raw).performModifier(mod_valueToLowerCase);

        m_currentCoinbase = spFH20(new FH20(m_raw->atKey("coinbase")));
        m_currentDifficulty = spVALUE(new VALUE(m_raw->atKey("difficulty")));
        m_currentGasLimit = spVALUE(new VALUE(m_raw->atKey("gasLimit")));
        if (m_currentGasLimit.getCContent() > dev::bigint("0x7fffffffffffffff"))
            throw test::UpwardsException("currentGasLimit must be < 0x7fffffffffffffff");

        if (m_raw->count("baseFeePerGas"))
        {
            (*m_raw).atKeyUnsafe("baseFeePerGas").performModifier(mod_valueToCompactEvenHexPrefixed);
            m_currentBaseFee = spVALUE(new VALUE(m_raw->atKey("baseFeePerGas")));
        }

        m_currentNumber = spVALUE(new VALUE(m_raw->atKey("number")));
        m_currentTimestamp = spVALUE(new VALUE(m_raw->atKey("timestamp")));
        m_previousHash = spFH32(new FH32(m_raw->atKey("parentHash")));
        m_currentExtraData = spBYTES(new BYTES(m_raw->atKey("extraData")));

        if (_sEngine == SealEngine::NoProof)
        {
            m_currentNonce = spFH8(new FH8(FH8::zero()));
            m_currentMixHash = spFH32(new FH32(FH32::zero()));
        }
        else
        {
            m_currentNonce = spFH8(new FH8(m_raw->atKey("nonce")));
            m_currentMixHash = spFH32(new FH32(m_raw->atKey("mixHash")));
        }

        // Allowed fields for this structure
        REQUIRE_JSONFIELDS(m_raw, "GenesisBlockHeader(BlockchainTestFillerEnv) " + m_raw->getKey(),
            {{"bloom", {{DataType::String}, jsonField::Optional}},
             {"logsBloom", {{DataType::String}, jsonField::Optional}},
             {"coinbase", {{DataType::String}, jsonField::Optional}},
             {"author", {{DataType::String}, jsonField::Optional}},
             {"miner", {{DataType::String}, jsonField::Optional}},
             {"difficulty", {{DataType::String}, jsonField::Required}},
             {"extraData", {{DataType::String}, jsonField::Required}},
             {"gasLimit", {{DataType::String}, jsonField::Required}},
             {"baseFeePerGas", {{DataType::String}, jsonField::Optional}},
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

        // Format the env to RPC format
        spDataObject out(new DataObject());
        (*out).atKeyPointer("currentCoinbase") = (*m_raw).atKeyPointerUnsafe("coinbase");
        (*out).atKeyPointer("currentDifficulty") = (*m_raw).atKeyPointerUnsafe("difficulty");
        (*out).atKeyPointer("currentNumber") = (*m_raw).atKeyPointerUnsafe("number");
        (*out).atKeyPointer("currentTimestamp") = (*m_raw).atKeyPointerUnsafe("timestamp");
        (*out).atKeyPointer("currentGasLimit") = (*m_raw).atKeyPointerUnsafe("gasLimit");
        (*out).atKeyPointer("previousHash") = (*m_raw).atKeyPointerUnsafe("parentHash");
        if (!m_currentBaseFee.isEmpty())
            (*out).atKeyPointer("currentBaseFee") = (*m_raw).atKeyPointerUnsafe("baseFeePerGas");
        m_raw = out;
    }
    catch (std::exception const& _ex)
    {
        throw UpwardsException(string("BlockchainTestFillerEnv convertion error: ") + _ex.what());
    }
}

}  // namespace teststruct
}  // namespace test
