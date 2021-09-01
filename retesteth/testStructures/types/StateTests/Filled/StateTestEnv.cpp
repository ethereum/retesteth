#include "StateTestEnv.h"
#include <retesteth/EthChecks.h>
#include <retesteth/testStructures/Common.h>
namespace test
{
namespace teststruct
{
// Env. Build a type representation
StateTestEnv::StateTestEnv(DataObject const& _data)
{
    try
    {
        m_currentCoinbase = spFH20(new FH20(_data.atKey("currentCoinbase")));
        m_currentDifficulty = spVALUE(new VALUE(_data.atKey("currentDifficulty")));
        m_currentNumber = spVALUE(new VALUE(_data.atKey("currentNumber")));
        m_currentGasLimit = spVALUE(new VALUE(_data.atKey("currentGasLimit")));

        // Indicates first block timestamp in StateTests
        m_currentTimestamp = spVALUE(new VALUE(_data.atKey("currentTimestamp")));
        // Indicates zero block timestamp in StateTests
        m_genesisTimestamp = spVALUE(new VALUE(0));
        m_previousHash = spFH32(new FH32(_data.atKey("previousHash")));

        DataObject tmpD;
        tmpD = "0x00";  // State Tests extra data is 0x00
        m_currentExtraData = spBYTES(new BYTES(tmpD));
        m_currentNonce = spFH8(new FH8(FH8::zero()));
        m_currentMixHash = spFH32(new FH32(FH32::zero()));

        if (_data.count("currentBaseFee"))
        {
            // 1559 env info
            m_currentBaseFee = spVALUE(new VALUE(_data.atKey("currentBaseFee")));
            REQUIRE_JSONFIELDS(_data, "StateTestEnv " + _data.getKey(),
                {{"currentCoinbase", {{DataType::String}, jsonField::Required}},
                {"currentDifficulty", {{DataType::String}, jsonField::Required}},
                {"currentNumber", {{DataType::String}, jsonField::Required}},
                {"currentTimestamp", {{DataType::String}, jsonField::Required}},
                {"currentGasLimit", {{DataType::String}, jsonField::Required}},
                {"currentBaseFee", {{DataType::String}, jsonField::Required}},
                {"previousHash", {{DataType::String}, jsonField::Required}}});
        }
        else
        {
            // Legacy env info
            m_currentBaseFee = spVALUE(0);
            REQUIRE_JSONFIELDS(_data, "StateTestEnv " + _data.getKey(),
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
        throw UpwardsException(string("StateTestEnv parse error: ") + _ex.what() + _data.asJson());
    }
}

}  // namespace teststruct
}  // namespace test
