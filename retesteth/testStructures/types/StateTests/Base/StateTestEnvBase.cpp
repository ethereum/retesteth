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
    out["currentCoinbase"] = m_currentCoinbase->asString();
    out["currentDifficulty"] = m_currentDifficulty->asString();
    out["currentNumber"] = m_currentNumber->asString();
    out["currentTimestamp"] = m_currentTimestamp->asString();
    out["previousHash"] = m_previousHash->asString();
    out["currentGasLimit"] = m_currentGasLimit->asString();

    if (!m_currentBaseFee.isEmpty())
    {
        // EIP1559 env info
        out["currentBaseFee"] = m_currentBaseFee->asString();
    }
    return out;
}

}  // namespace teststruct
}  // namespace test
