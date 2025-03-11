#include "StateTestEnv.h"
#include <retesteth/EthChecks.h>
#include <retesteth/testStructures/Common.h>
#include <retesteth/Constants.h>
#include "Options.h"

using namespace std;
using namespace dataobject;
using namespace test::teststruct;
using namespace test::teststruct::constnames;

namespace
{
void requireStateTestEnvScheme(DataObject const& _data)
{
    auto const& opt = test::Options::get();
    if (opt.isLegacy())
    {
        REQUIRE_JSONFIELDS(_data, "StateTestEnv(Legacy) " + _data.getKey(),
            {{"currentCoinbase", {{DataType::String}, jsonField::Required}},
                {"currentDifficulty", {{DataType::String}, jsonField::Optional}},
                {"currentGasLimit", {{DataType::String}, jsonField::Required}},
                {"currentNumber", {{DataType::String}, jsonField::Required}},
                {"currentTimestamp", {{DataType::String}, jsonField::Required}},
                {"currentBaseFee", {{DataType::String}, jsonField::Optional}},
                {"currentRandom", {{DataType::String}, jsonField::Optional}},
                {c_currentExcessBlobGas, {{DataType::String}, jsonField::Optional}},
                {"previousHash", {{DataType::String}, jsonField::Optional}}
            });
    }
    else
    {
        REQUIRE_JSONFIELDS(_data, "StateTestEnv " + _data.getKey(),
            {{"currentCoinbase", {{DataType::String}, jsonField::Required}},
                {"currentDifficulty", {{DataType::String}, jsonField::Optional}},
                {"currentGasLimit", {{DataType::String}, jsonField::Required}},
                {"currentNumber", {{DataType::String}, jsonField::Required}},
                {"currentTimestamp", {{DataType::String}, jsonField::Required}},
                {"currentBaseFee", {{DataType::String}, jsonField::Optional}},
                {"currentRandom", {{DataType::String}, jsonField::Optional}},
                {c_currentExcessBlobGas, {{DataType::String}, jsonField::Optional}},
                {c_currentRequestsHash, {{DataType::String}, jsonField::Optional}}
            });
    }
}

}  // namespace


namespace test::teststruct
{
void StateTestEnv::initializeFields(DataObject const& _data)
{
    m_currentCoinbase = sFH20(_data.atKey("currentCoinbase"));
    m_currentNumber = sVALUE(_data.atKey("currentNumber"));
    m_currentGasLimit = sVALUE(_data.atKey("currentGasLimit"));

    // Indicates first block timestamp in StateTests
    m_currentTimestamp = sVALUE(_data.atKey("currentTimestamp"));
    // Indicates zero block timestamp in StateTests
    m_genesisTimestamp = sVALUE(0);

    DataObject tmpD;
    tmpD = "0x00";  // State Tests extra data is 0x00
    m_currentExtraData = sBYTES(tmpD);
    m_currentNonce = spFH8(FH8::zero().copy());
    m_currentMixHash = spFH32(FH32::zero().copy());

    // 1559
    m_currentDifficulty = sVALUE(DataObject("0x00"));
    m_currentBaseFee = sVALUE(DataObject("0x0a"));

    if (_data.count("currentBaseFee"))
        m_currentBaseFee = sVALUE(_data.atKey("currentBaseFee"));
    if (_data.count("currentDifficulty"))
        m_currentDifficulty = sVALUE(_data.atKey("currentDifficulty"));

    // Paris
    m_currentRandom = spFH32(FH32::zero().copy());
    if (_data.count("currentRandom"))
        m_currentRandom = sFH32(_data.atKey("currentRandom"));

    // Shanghai
    m_currentWithdrawalsRoot = sFH32(DataObject(C_WITHDRAWALS_EMPTY_ROOT));

    // Cancun
    m_currentBlobGasUsed = sVALUE(DataObject("0x00"));
    m_currentExcessBlobGas = sVALUE(DataObject("0x00"));
    if (_data.count(c_currentExcessBlobGas))
        m_currentExcessBlobGas = sVALUE(_data.atKey(c_currentExcessBlobGas));
    m_currentBlobGasUsed = sVALUE(DataObject("0x00"));
    //if (_data.count(c_parentBlobGasUsed))
    //    m_currentBlobGasUsed = sVALUE(_data.atKey(c_parentBlobGasUsed));
    m_currentBeaconRoot = spFH32(FH32::zero().copy());
    if (_data.count(c_currentBeaconRoot))
        m_currentBeaconRoot = sFH32(_data.atKey(c_currentBeaconRoot));

    m_currentRequestsHash = spFH32(C_FH32_DEFAULT_REQUESTS_HASH.copy());
    if (_data.count(c_currentRequestsHash))
        m_currentRequestsHash = sFH32(_data.atKey(c_currentRequestsHash));
}


StateTestEnv::StateTestEnv(DataObject const& _data)
{
    try
    {
        requireStateTestEnvScheme(_data);
        initializeFields(_data);
    }
    catch (std::exception const& _ex)
    {
        throw UpwardsException(string("StateTestEnv parse error: ") + _ex.what() + _data.asJson());
    }
}

}  // namespace teststruct
