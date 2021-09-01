#include "StateTestFillerEnv.h"
#include <retesteth/EthChecks.h>
#include <retesteth/testStructures/Common.h>
namespace test
{
namespace teststruct
{
// Env. Build a type representation
StateTestFillerEnv::StateTestFillerEnv(spDataObjectMove _data)
{
    try
    {
        m_raw = _data.getPointer();

        (*m_raw).performModifier(mod_valueToCompactEvenHexPrefixed, {"currentCoinbase", "previousHash"});
        (*m_raw).atKeyUnsafe("currentCoinbase").performModifier(mod_valueInsertZeroXPrefix);
        (*m_raw).atKeyUnsafe("previousHash").performModifier(mod_valueInsertZeroXPrefix);
        (*m_raw).performModifier(mod_valueToLowerCase);

        m_currentCoinbase = spFH20(new FH20(m_raw->atKey("currentCoinbase")));
        m_currentDifficulty = spVALUE(new VALUE(m_raw->atKey("currentDifficulty")));
        m_currentNumber = spVALUE(new VALUE(m_raw->atKey("currentNumber")));

        // Indicates first block timestamp in StateTests
        m_currentTimestamp = spVALUE(new VALUE(m_raw->atKey("currentTimestamp")));
        // Indicates zero block timestamp in StateTests
        m_genesisTimestamp = spVALUE(new VALUE(0));

        // Do not allow hash without 0x
        m_previousHash = spFH32(new FH32(m_raw->atKey("previousHash")));

        spDataObject tmpD(new DataObject("0x00"));  // State Tests extra data is 0x00
        m_currentExtraData = spBYTES(new BYTES(tmpD));
        m_currentNonce = spFH8(new FH8(FH8::zero()));
        m_currentMixHash = spFH32(new FH32(FH32::zero()));
        m_currentGasLimit = spVALUE(new VALUE(m_raw->atKey("currentGasLimit")));
        if (m_currentGasLimit.getCContent() > dev::bigint("0x7fffffffffffffff"))
            throw test::UpwardsException("currentGasLimit > 0x7fffffffffffffff");

        if (m_raw->count("currentBaseFee"))
        {
            // 1559 env info
            m_currentBaseFee = spVALUE(new VALUE(m_raw->atKey("currentBaseFee")));
            REQUIRE_JSONFIELDS(m_raw, "StateTestFillerEnv " + m_raw->getKey(),
                {{"currentCoinbase", {{DataType::String}, jsonField::Required}},
                    {"currentDifficulty", {{DataType::String}, jsonField::Required}},
                    {"currentGasLimit", {{DataType::String}, jsonField::Required}},
                    {"currentNumber", {{DataType::String}, jsonField::Required}},
                    {"currentTimestamp", {{DataType::String}, jsonField::Required}},
                    {"currentBaseFee", {{DataType::String}, jsonField::Required}},
                    {"previousHash", {{DataType::String}, jsonField::Required}}});
        }
        else
        {
            // legacy env info
            m_currentBaseFee = spVALUE(0);
            REQUIRE_JSONFIELDS(m_raw, "StateTestFillerEnv " + m_raw->getKey(),
                {{"currentCoinbase", {{DataType::String}, jsonField::Required}},
                    {"currentDifficulty", {{DataType::String}, jsonField::Required}},
                    {"currentGasLimit", {{DataType::String}, jsonField::Required}},
                    {"currentNumber", {{DataType::String}, jsonField::Required}},
                    {"currentTimestamp", {{DataType::String}, jsonField::Required}},
                    {"previousHash", {{DataType::String}, jsonField::Required}}});
        }
    }
    catch (std::exception const& _ex)
    {
        throw UpwardsException(string("StateTestFillerEnv parse error: ") + _ex.what() + m_raw->asJson());
    }
}

}  // namespace teststruct
}  // namespace test
