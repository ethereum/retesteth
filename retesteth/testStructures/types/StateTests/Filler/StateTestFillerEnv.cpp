#include "StateTestFillerEnv.h"
#include <retesteth/EthChecks.h>
#include <retesteth/testStructures/Common.h>

using namespace std;
using namespace dataobject;
using namespace test::teststruct;
namespace
{
void requireStateTestsFillerEnvScheme(spDataObject const& _data)
{
    REQUIRE_JSONFIELDS(_data, "StateTestFillerEnv " + _data->getKey(),
        {{"currentCoinbase", {{DataType::String}, jsonField::Required}},
         {"currentDifficulty", {{DataType::String}, jsonField::Optional}},
         {"currentGasLimit", {{DataType::String}, jsonField::Required}},
         {"currentNumber", {{DataType::String}, jsonField::Required}},
         {"currentTimestamp", {{DataType::String}, jsonField::Required}},
         {"currentBaseFee", {{DataType::String}, jsonField::Optional}},
         {"currentRandom", {{DataType::String}, jsonField::Optional}},
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
StateTestFillerEnv::StateTestFillerEnv(spDataObjectMove _data)
{
    try
    {
        m_raw = _data.getPointer();
        requireStateTestsFillerEnvScheme(m_raw);
        convertEnvDecFieldsToHex(m_raw);
        initializeFields(m_raw);
    }
    catch (std::exception const& _ex)
    {
        throw UpwardsException(string("StateTestFillerEnv parse error: ") + _ex.what() + m_raw->asJson());
    }
}

void StateTestFillerEnv::initializeFields(spDataObject const& _data)
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

    // 1559
    m_currentDifficulty = spVALUE(new VALUE(DataObject("0x020000")));
    m_currentBaseFee = spVALUE(new VALUE(DataObject("0x0a")));

    if (_data->count("currentDifficulty"))
        m_currentDifficulty = spVALUE(new VALUE(_data->atKey("currentDifficulty")));

    if (_data->count("currentBaseFee"))
        m_currentBaseFee = spVALUE(new VALUE(_data->atKey("currentBaseFee")));

    // Merge
    auto const& difficulty = m_currentDifficulty->asString();
    m_currentRandom = spFH32(new FH32(dev::toCompactHexPrefixed(dev::u256(difficulty), 32)));

    if (_data->count("currentRandom"))
        m_currentRandom = spFH32(new FH32(_data->atKey("currentRandom")));
}

spDataObject const& StateTestFillerEnv::asDataObject() const
{
    spDataObject const& c_raw = StateTestEnvBase::asDataObject();
    spDataObject& raw = const_cast<spDataObject&>(c_raw);
    if (!raw->count("currentBaseFee"))
        (*raw)["currentBaseFee"] = m_currentBaseFee.getCContent().asString();

    if (!raw->count("currentRandom"))
        (*raw)["currentRandom"] = m_currentRandom.getCContent().asString();

    if (!raw->count("currentDifficulty"))
        (*raw)["currentDifficulty"] = m_currentDifficulty.getCContent().asString();

    return raw;
}

}  // namespace teststruct
}  // namespace test
