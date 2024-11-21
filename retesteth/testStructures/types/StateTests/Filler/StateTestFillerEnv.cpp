#include "StateTestFillerEnv.h"
#include <retesteth/EthChecks.h>
#include <retesteth/testStructures/Common.h>
#include <retesteth/Constants.h>
#include <retesteth/Options.h>

using namespace std;
using namespace dataobject;
using namespace test::teststruct;
using namespace test::teststruct::constnames;
namespace
{
void requireStateTestsFillerEnvScheme(spDataObject const& _data)
{
    auto const& opt = test::Options::get();
    if (opt.isLegacy())
    {
        REQUIRE_JSONFIELDS(_data, "StateTestFillerEnv(Legacy) " + _data->getKey(),
            {{"currentCoinbase", {{DataType::String}, jsonField::Required}},
                {"currentDifficulty", {{DataType::String}, jsonField::Optional}},
                {"currentGasLimit", {{DataType::String}, jsonField::Required}},
                {"currentNumber", {{DataType::String}, jsonField::Required}},
                {"currentTimestamp", {{DataType::String}, jsonField::Required}},
                {"currentBaseFee", {{DataType::String}, jsonField::Optional}},
                {"currentRandom", {{DataType::String}, jsonField::Optional}},
                {"previousHash", {{DataType::String}, jsonField::Optional}},
                {c_currentExcessBlobGas, {{DataType::String}, jsonField::Optional}}
            });
    }
    else
    {
        REQUIRE_JSONFIELDS(_data, "StateTestFillerEnv " + _data->getKey(),
            {{"currentCoinbase", {{DataType::String}, jsonField::Required}},
             {"currentDifficulty", {{DataType::String}, jsonField::Optional}},
             {"currentGasLimit", {{DataType::String}, jsonField::Required}},
             {"currentNumber", {{DataType::String}, jsonField::Required}},
             {"currentTimestamp", {{DataType::String}, jsonField::Required}},
             {"currentBaseFee", {{DataType::String}, jsonField::Optional}},
             {"currentRandom", {{DataType::String}, jsonField::Optional}},
             {c_currentExcessBlobGas, {{DataType::String}, jsonField::Optional}}
        });
    }
}

void convertEnvDecFieldsToHex(spDataObject& _data)
{
    (*_data).performModifier(mod_valueToCompactEvenHexPrefixed, DataObject::ModifierOption::RECURSIVE,
        {"currentCoinbase", "previousHash", "currentRandom"});
    (*_data).atKeyUnsafe("currentCoinbase").performModifier(mod_valueInsertZeroXPrefix);
    (*_data).performModifier(mod_valueToLowerCase);
}
}  // namespace


namespace test::teststruct
{

StateTestFillerEnv::StateTestFillerEnv(spDataObjectMove _data)
{
    spDataObject data = _data.getPointer();
    try
    {
        requireStateTestsFillerEnvScheme(data);
        convertEnvDecFieldsToHex(data);
        initializeFields(data);
    }
    catch (std::exception const& _ex)
    {
        throw UpwardsException(string("StateTestFillerEnv parse error: ") + _ex.what() + data->asJson());
    }
}

void StateTestFillerEnv::initializeFields(spDataObject const& _data)
{
    m_currentNumber = sVALUE(_data->atKey("currentNumber"));
    m_currentCoinbase = sFH20(_data->atKey("currentCoinbase"));

    // Indicates first block timestamp in StateTests
    m_currentTimestamp = sVALUE(_data->atKey("currentTimestamp"));
    // Indicates zero block timestamp in StateTests
    m_genesisTimestamp = sVALUE(0);

    spDataObject tmpD(new DataObject("0x00"));  // State Tests extra data is 0x00
    m_currentExtraData = sBYTES(tmpD);
    m_currentNonce = spFH8(FH8::zero().copy());
    m_currentMixHash = spFH32(FH32::zero().copy());
    m_currentGasLimit = sVALUE(_data->atKey("currentGasLimit"));
    if (m_currentGasLimit.getCContent() > dev::bigint("0x7fffffffffffffff"))
        throw test::UpwardsException("currentGasLimit > 0x7fffffffffffffff");

    // 1559
    m_currentDifficulty = sVALUE(DataObject("0x020000"));
    m_currentBaseFee = sVALUE(DataObject("0x0a"));

    if (_data->count("currentDifficulty"))
        m_currentDifficulty = sVALUE(_data->atKey("currentDifficulty"));

    if (_data->count("currentBaseFee"))
        m_currentBaseFee = sVALUE(_data->atKey("currentBaseFee"));

    // Paris
    auto const& difficulty = m_currentDifficulty->asString();
    m_currentRandom = sFH32(dev::toCompactHexPrefixed(dev::u256(difficulty), 32));

    if (_data->count("currentRandom"))
        m_currentRandom = sFH32(_data->atKey("currentRandom"));

    // Shanghai
    m_currentWithdrawalsRoot = sFH32(DataObject(C_WITHDRAWALS_EMPTY_ROOT));

    // Cancun
    m_currentBlobGasUsed = sVALUE(DataObject("0x00"));
    m_currentBeaconRoot = spFH32(FH32::zero().copy());

    m_currentExcessBlobGas = sVALUE(DataObject("0x00"));
    if (_data->count(c_currentExcessBlobGas))
        m_currentExcessBlobGas = sVALUE(_data->atKey(c_currentExcessBlobGas));
    if (_data->count(c_parentBlobGasUsed))
        m_currentBlobGasUsed = sVALUE(_data->atKey(c_parentBlobGasUsed));
    if (_data->count(c_currentBeaconRoot))
        m_currentBeaconRoot = sFH32(_data->atKey(c_currentBeaconRoot));

    // Prague
    m_currentRequestsHash = spFH32(C_FH32_DEFAULT_REQUESTS_HASH.copy());
}

}  // namespace teststruct
