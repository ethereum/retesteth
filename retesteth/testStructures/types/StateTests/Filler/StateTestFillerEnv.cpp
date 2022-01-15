#include "StateTestFillerEnv.h"
#include <retesteth/EthChecks.h>
#include <retesteth/testStructures/Common.h>

namespace
{
void requireLegacyEnvScheme(spDataObject const& _data)
{
    REQUIRE_JSONFIELDS(_data, "StateTestFillerEnv(Legacy) " + _data->getKey(),
        {{"currentCoinbase", {{DataType::String}, jsonField::Required}},
            {"currentDifficulty", {{DataType::String}, jsonField::Required}},
            {"currentGasLimit", {{DataType::String}, jsonField::Required}},
            {"currentNumber", {{DataType::String}, jsonField::Required}},
            {"currentTimestamp", {{DataType::String}, jsonField::Required}},
            {"previousHash", {{DataType::String}, jsonField::Required}}});
}

void requireEIP1559EnvScheme(spDataObject const& _data)
{
    REQUIRE_JSONFIELDS(_data, "StateTestFillerEnv(EIP1559) " + _data->getKey(),
        {{"currentCoinbase", {{DataType::String}, jsonField::Required}},
            {"currentDifficulty", {{DataType::String}, jsonField::Required}},
            {"currentGasLimit", {{DataType::String}, jsonField::Required}},
            {"currentNumber", {{DataType::String}, jsonField::Required}},
            {"currentTimestamp", {{DataType::String}, jsonField::Required}},
            {"currentBaseFee", {{DataType::String}, jsonField::Required}},
            {"previousHash", {{DataType::String}, jsonField::Required}}});
}

void requireMergeEnvScheme(spDataObject const& _data)
{
    REQUIRE_JSONFIELDS(_data, "StateTestFillerEnv(Merge) " + _data->getKey(),
        {{"currentCoinbase", {{DataType::String}, jsonField::Required}},
            {"currentGasLimit", {{DataType::String}, jsonField::Required}},
            {"currentNumber", {{DataType::String}, jsonField::Required}},
            {"currentTimestamp", {{DataType::String}, jsonField::Required}},
            {"currentBaseFee", {{DataType::String}, jsonField::Required}},
            {"currentRandom", {{DataType::String}, jsonField::Required}},
            {"previousHash", {{DataType::String}, jsonField::Required}}});
}

void convertEnvDecFieldsToHex(spDataObject& _data)
{
    (*_data).performModifier(mod_valueToCompactEvenHexPrefixed, DataObject::ModifierOption::RECURSIVE,
        {"currentCoinbase", "previousHash", "currentRandom"});
    (*_data).atKeyUnsafe("currentCoinbase").performModifier(mod_valueInsertZeroXPrefix);
    (*_data).atKeyUnsafe("previousHash").performModifier(mod_valueInsertZeroXPrefix);
    (*_data).performModifier(mod_valueToLowerCase);
}
}  // namespace


namespace test
{
namespace teststruct
{
void StateTestFillerEnvMerge::initializeMergeFields(DataObject const& _data)
{
    m_currentBaseFee = spVALUE(new VALUE(_data.atKey("currentBaseFee")));
    m_currentRandom = spFH32(new FH32(_data.atKey("currentRandom")));
}

void StateTestFillerEnv1559::initialize1559Fields(DataObject const& _data)
{
    m_currentDifficulty = spVALUE(new VALUE(_data.atKey("currentDifficulty")));
    m_currentBaseFee = spVALUE(new VALUE(_data.atKey("currentBaseFee")));
}

void StateTestFillerEnvLegacy::initializeLegacyFields(DataObject const& _data)
{
    m_currentDifficulty = spVALUE(new VALUE(_data.atKey("currentDifficulty")));
}

StateTestFillerEnvMerge::StateTestFillerEnvMerge(spDataObjectMove _data)
{
    try
    {
        m_raw = _data.getPointer();
        requireMergeEnvScheme(m_raw);
        convertEnvDecFieldsToHex(m_raw);
        initializeCommonFields(m_raw);
        initializeMergeFields(m_raw);
    }
    catch (std::exception const& _ex)
    {
        throw UpwardsException(string("StateTestFillerEnv(Merge) parse error: ") + _ex.what() + m_raw->asJson());
    }
}

StateTestFillerEnv1559::StateTestFillerEnv1559(spDataObjectMove _data)
{
    try
    {
        m_raw = _data.getPointer();
        requireEIP1559EnvScheme(m_raw);
        convertEnvDecFieldsToHex(m_raw);
        initializeCommonFields(m_raw);
        initialize1559Fields(m_raw);
    }
    catch (std::exception const& _ex)
    {
        throw UpwardsException(string("StateTestFillerEnv(1559) parse error: ") + _ex.what() + m_raw->asJson());
    }
}

StateTestFillerEnvLegacy::StateTestFillerEnvLegacy(spDataObjectMove _data)
{
    try
    {
        m_raw = _data.getPointer();
        requireLegacyEnvScheme(m_raw);
        convertEnvDecFieldsToHex(m_raw);
        initializeCommonFields(m_raw);
        initializeLegacyFields(m_raw);
    }
    catch (std::exception const& _ex)
    {
        throw UpwardsException(string("StateTestFillerEnv(legacy) parse error: ") + _ex.what() + m_raw->asJson());
    }
}

void StateTestFillerEnv::initializeCommonFields(spDataObject const& _data)
{
    m_currentNumber = spVALUE(new VALUE(_data->atKey("currentNumber")));
    m_currentCoinbase = spFH20(new FH20(_data->atKey("currentCoinbase")));
    // Indicates first block timestamp in StateTests
    m_currentTimestamp = spVALUE(new VALUE(_data->atKey("currentTimestamp")));
    // Indicates zero block timestamp in StateTests
    m_genesisTimestamp = spVALUE(new VALUE(0));
    m_previousHash = spFH32(new FH32(_data->atKey("previousHash")));

    spDataObject tmpD(new DataObject("0x00"));  // State Tests extra data is 0x00
    m_currentExtraData = spBYTES(new BYTES(tmpD));
    m_currentNonce = spFH8(new FH8(FH8::zero()));
    m_currentMixHash = spFH32(new FH32(FH32::zero()));
    m_currentGasLimit = spVALUE(new VALUE(_data->atKey("currentGasLimit")));
    if (m_currentGasLimit.getCContent() > dev::bigint("0x7fffffffffffffff"))
        throw test::UpwardsException("currentGasLimit > 0x7fffffffffffffff");
}

}  // namespace teststruct
}  // namespace test
