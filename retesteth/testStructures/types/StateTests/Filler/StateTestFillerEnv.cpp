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
        m_currentCoinbase = spFH20(new FH20(tmpData.atKey("currentCoinbase")));
        m_currentDifficulty = spVALUE(new VALUE(tmpData.atKey("currentDifficulty")));
        m_currentGasLimit = spVALUE(new VALUE(tmpData.atKey("currentGasLimit"), dev::u256("0x7fffffffffffffff")));
        m_currentNumber = spVALUE(new VALUE(tmpData.atKey("currentNumber")));
        m_currentTimestamp = spVALUE(new VALUE(tmpData.atKey("currentTimestamp")));

        // Do not allow hash without 0x
        m_previousHash = spFH32(new FH32(_data.atKey("previousHash")));
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
    out["currentCoinbase"] = m_currentCoinbase.getCContent().asString();
    out["currentDifficulty"] = m_currentDifficulty.getCContent().asString();
    out["currentGasLimit"] = m_currentGasLimit.getCContent().asString();
    out["currentNumber"] = m_currentNumber.getCContent().asString();
    out["currentTimestamp"] = m_currentTimestamp.getCContent().asString();
    out["previousHash"] = m_previousHash.getCContent().asString();
    return out;
}

}  // namespace teststruct
}  // namespace test
