#include "StateTestFillerEnv.h"
#include <retesteth/EthChecks.h>
#include <retesteth/testStructures/Common.h>
namespace test
{
namespace teststruct
{
// Env. Build a type representation
StateTestFillerEnv::StateTestFillerEnv(DataObject const& _data)
{
    try
    {
        spDataObject tmpData(new DataObject());
        (*tmpData).copyFrom(_data);
        (*tmpData).removeKey("currentCoinbase");
        (*tmpData).performModifier(mod_valueToCompactEvenHexPrefixed);

        spDataObject coinbase(new DataObject());
        (*coinbase).copyFrom(_data.atKey("currentCoinbase"));
        if (coinbase->asString().size() > 1 && coinbase->asString()[1] != 'x')
            (*coinbase) = "0x" + coinbase->asString();
        m_currentCoinbase = spFH20(new FH20(coinbase));

        m_currentDifficulty = spVALUE(new VALUE(tmpData->atKey("currentDifficulty")));
        m_currentNumber = spVALUE(new VALUE(tmpData->atKey("currentNumber")));

        // Indicates first block timestamp in StateTests
        m_currentTimestamp = spVALUE(new VALUE(tmpData->atKey("currentTimestamp")));
        // Indicates zero block timestamp in StateTests
        m_genesisTimestamp = spVALUE(new VALUE(0));

        // Do not allow hash without 0x
        m_previousHash = spFH32(new FH32(tmpData->atKey("previousHash")));

        spDataObject tmpD(new DataObject());
        (*tmpD) = "0x00";  // State Tests extra data is 0x00
        m_currentExtraData = spBYTES(new BYTES(tmpD));
        m_currentNonce = spFH8(new FH8(FH8::zero()));
        m_currentMixHash = spFH32(new FH32(FH32::zero()));
        m_currentGasLimit = spVALUE(new VALUE(tmpData->atKey("currentGasLimit")));
        if (m_currentGasLimit.getCContent() > dev::bigint("0x7fffffffffffffff"))
            throw test::UpwardsException("currentGasLimit > 0x7fffffffffffffff");

        if (_data.count("currentBaseFee"))
        {
            // 1559 env info
            m_currentBaseFee = spVALUE(new VALUE(tmpData->atKey("currentBaseFee")));
            requireJsonFields(_data, "StateTestFillerEnv " + _data.getKey(),
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
            requireJsonFields(_data, "StateTestFillerEnv " + _data.getKey(),
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
        throw UpwardsException(string("StateTestFillerEnv parse error: ") + _ex.what() + _data.asJson());
    }
}

}  // namespace teststruct
}  // namespace test
