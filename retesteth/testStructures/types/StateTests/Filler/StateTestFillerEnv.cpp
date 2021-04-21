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
        DataObject tmpData = _data;
        tmpData.removeKey("currentCoinbase");
        tmpData.performModifier(mod_valueToCompactEvenHexPrefixed);

        DataObject coinbase = _data.atKey("currentCoinbase");
        if (coinbase.asString().size() > 1 && coinbase.asString()[1] != 'x')
            coinbase = "0x" + coinbase.asString();
        m_currentCoinbase = spFH20(new FH20(coinbase));

        m_currentDifficulty = spVALUE(new VALUE(tmpData.atKey("currentDifficulty")));
        m_currentGasLimit = spVALUE(new VALUE(tmpData.atKey("currentGasLimit")));
        if (m_currentGasLimit.getCContent() > dev::u256("0x7fffffffffffffff"))
            throw test::UpwardsException("currentGasLimit > 0x7fffffffffffffff");
        m_currentNumber = spVALUE(new VALUE(tmpData.atKey("currentNumber")));

        // Indicates first block timestamp in StateTests
        m_currentTimestamp = spVALUE(new VALUE(tmpData.atKey("currentTimestamp")));
        // Indicates zero block timestamp in StateTests
        m_genesisTimestamp = spVALUE(new VALUE(0));

        // Do not allow hash without 0x
        m_previousHash = spFH32(new FH32(tmpData.atKey("previousHash")));

        DataObject tmpD;
        tmpD = "0x00";  // State Tests extra data is 0x00
        m_currentExtraData = spBYTES(new BYTES(tmpD));
        m_currentNonce = spFH8(new FH8(FH8::zero()));
        m_currentMixHash = spFH32(new FH32(FH32::zero()));

        if (_data.count("gasTarget"))
        {
            m_gasTarget = spVALUE(new VALUE(tmpData.atKey("gasTarget")));
            m_baseFeePerGas = spVALUE(new VALUE(tmpData.atKey("baseFeePerGas")));
        }

        requireJsonFields(_data, "StateTestFillerEnv " + _data.getKey(),
            {{"currentCoinbase", {{DataType::String}, jsonField::Required}},
                {"currentDifficulty", {{DataType::String}, jsonField::Required}},
                {"currentGasLimit", {{DataType::String}, jsonField::Required}},
                {"currentNumber", {{DataType::String}, jsonField::Required}},
                {"currentTimestamp", {{DataType::String}, jsonField::Required}},
                {"gasTarget", {{DataType::String}, jsonField::Optional}},
                {"baseFeePerGas", {{DataType::String}, jsonField::Optional}},
                {"previousHash", {{DataType::String}, jsonField::Required}}});
    }
    catch (std::exception const& _ex)
    {
        throw UpwardsException(string("StateTestFillerEnv parse error: ") + _ex.what() + _data.asJson());
    }
}

}  // namespace teststruct
}  // namespace test
