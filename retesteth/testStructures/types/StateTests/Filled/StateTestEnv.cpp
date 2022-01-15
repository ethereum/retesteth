#include "StateTestEnv.h"
#include <retesteth/EthChecks.h>
#include <retesteth/testStructures/Common.h>

namespace
{
void requireLegacyEnvScheme(DataObject const& _data)
{
    REQUIRE_JSONFIELDS(_data, "StateTestEnv(Legacy) " + _data.getKey(),
        {{"currentCoinbase", {{DataType::String}, jsonField::Required}},
            {"currentDifficulty", {{DataType::String}, jsonField::Required}},
            {"currentGasLimit", {{DataType::String}, jsonField::Required}},
            {"currentNumber", {{DataType::String}, jsonField::Required}},
            {"currentTimestamp", {{DataType::String}, jsonField::Required}},
            {"previousHash", {{DataType::String}, jsonField::Required}}});
}

void requireEIP1559EnvScheme(DataObject const& _data)
{
    REQUIRE_JSONFIELDS(_data, "StateTestEnv(1559) " + _data.getKey(),
        {{"currentCoinbase", {{DataType::String}, jsonField::Required}},
            {"currentDifficulty", {{DataType::String}, jsonField::Required}},
            {"currentNumber", {{DataType::String}, jsonField::Required}},
            {"currentTimestamp", {{DataType::String}, jsonField::Required}},
            {"currentGasLimit", {{DataType::String}, jsonField::Required}},
            {"currentBaseFee", {{DataType::String}, jsonField::Required}},
            {"previousHash", {{DataType::String}, jsonField::Required}}});
}

void requireMergeEnvScheme(DataObject const& _data)
{
    REQUIRE_JSONFIELDS(_data, "StateTestEnv(Merge) " + _data.getKey(),
        {{"currentCoinbase", {{DataType::String}, jsonField::Required}},
            {"currentRandom", {{DataType::String}, jsonField::Required}},
            {"currentNumber", {{DataType::String}, jsonField::Required}},
            {"currentTimestamp", {{DataType::String}, jsonField::Required}},
            {"currentGasLimit", {{DataType::String}, jsonField::Required}},
            {"currentBaseFee", {{DataType::String}, jsonField::Required}},
            {"previousHash", {{DataType::String}, jsonField::Required}}});
}

}  // namespace


namespace test
{
namespace teststruct
{
void StateTestEnv::initializeCommonFields(DataObject const& _data)
{
    m_currentCoinbase = spFH20(new FH20(_data.atKey("currentCoinbase")));
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
}

void StateTestEnvMerge::initializeMergeFields(DataObject const& _data)
{
    m_currentRandom = spFH32(new FH32(_data.atKey("currentRandom")));
    m_currentBaseFee = spVALUE(new VALUE(_data.atKey("currentBaseFee")));
}

void StateTestEnv1559::initialize1559Fields(DataObject const& _data)
{
    m_currentDifficulty = spVALUE(new VALUE(_data.atKey("currentDifficulty")));
    m_currentBaseFee = spVALUE(new VALUE(_data.atKey("currentBaseFee")));
}

void StateTestEnvLegacy::initializeLegacyFields(DataObject const& _data)
{
    m_currentDifficulty = spVALUE(new VALUE(_data.atKey("currentDifficulty")));
}

StateTestEnvMerge::StateTestEnvMerge(DataObject const& _data)
{
    try
    {
        requireMergeEnvScheme(_data);
        initializeCommonFields(_data);
        initializeMergeFields(_data);
    }
    catch (std::exception const& _ex)
    {
        throw UpwardsException(string("StateTestEnv(Merge) parse error: ") + _ex.what() + _data.asJson());
    }
}

StateTestEnv1559::StateTestEnv1559(DataObject const& _data)
{
    try
    {
        requireEIP1559EnvScheme(_data);
        initializeCommonFields(_data);
        initialize1559Fields(_data);
    }
    catch (std::exception const& _ex)
    {
        throw UpwardsException(string("StateTestEnv(1559) parse error: ") + _ex.what() + _data.asJson());
    }
}

StateTestEnvLegacy::StateTestEnvLegacy(DataObject const& _data)
{
    try
    {
        requireLegacyEnvScheme(_data);
        initializeCommonFields(_data);
        initializeLegacyFields(_data);
    }
    catch (std::exception const& _ex)
    {
        throw UpwardsException(string("StateTestEnv(1559) parse error: ") + _ex.what() + _data.asJson());
    }
}


}  // namespace teststruct
}  // namespace test
