#include "StateTestEnvBase.h"
#include <retesteth/EthChecks.h>
namespace test
{
namespace teststruct
{
// Export Env Function Some formatters might be performed here
const DataObject StateTestEnvBase::asDataObject() const
{
    DataObject out;
    out["currentCoinbase"] = m_currentCoinbase.getCContent().asString();
    out["currentDifficulty"] = m_currentDifficulty.getCContent().asString();
    out["currentNumber"] = m_currentNumber.getCContent().asString();
    out["currentTimestamp"] = m_currentTimestamp.getCContent().asString();
    out["previousHash"] = m_previousHash.getCContent().asString();

    if (!m_currentGasTarget.isEmpty())
    {
        // EIP1559 env info
        out["currentGasTarget"] = m_currentGasTarget.getCContent().asString();
        out["currentBaseFee"] = m_currentBaseFee.getCContent().asString();
    }
    else
    {
        // Legacy env info
        out["currentGasLimit"] = m_currentGasLimit.getCContent().asString();
    }
    return out;
}

}  // namespace teststruct
}  // namespace test
