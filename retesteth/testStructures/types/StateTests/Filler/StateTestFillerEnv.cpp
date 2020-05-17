#include "StateTestFillerEnv.h"
#include <retesteth/EthChecks.h>
#include <retesteth/ethObjects/object.h>
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
        tmpData.performModifier(mod_valueToCompactEvenHexPrefixed);
        m_currentCoinbase = GCP_SPointer<FH20>(new FH20(tmpData.atKey("currentCoinbase")));
        m_currentDifficulty = GCP_SPointer<VALUE>(new VALUE(tmpData.atKey("currentDifficulty")));
        m_currentGasLimit = GCP_SPointer<VALUE>(
            new VALUE(tmpData.atKey("currentGasLimit"), dev::u256("0x7fffffffffffffff")));
        m_currentNumber = GCP_SPointer<VALUE>(new VALUE(tmpData.atKey("currentNumber")));
        m_currentTimestamp = GCP_SPointer<VALUE>(new VALUE(tmpData.atKey("currentTimestamp")));

        // Do not allow hash without 0x
        m_previousHash = GCP_SPointer<FH32>(new FH32(_data.atKey("previousHash")));
    }
    catch (DataObjectException const& _ex)
    {
        ETH_ERROR_MESSAGE(_ex.what());
    }
}

// Export Env Function Some formatters might be performed here
DataObject StateTestFillerEnv::asDataObject() const
{
    DataObject out;
    out["currentCoinbase"] = m_currentCoinbase.getPointer()->asString();
    out["currentDifficulty"] = m_currentDifficulty.getPointer()->asString();
    out["currentGasLimit"] = m_currentGasLimit.getPointer()->asString();
    out["currentNumber"] = m_currentNumber.getPointer()->asString();
    out["currentTimestamp"] = m_currentTimestamp.getPointer()->asString();
    out["previousHash"] = m_previousHash.getPointer()->asString();
    return out;
}

}  // namespace teststruct
}  // namespace test
