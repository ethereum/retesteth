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
    out["currentGasLimit"] = m_currentGasLimit.getCContent().asString();

    if (!m_currentBaseFee.isEmpty())
    {
        // EIP1559 env info
        out["currentBaseFee"] = m_currentBaseFee.getCContent().asString();
    }
    return out;
}

}  // namespace teststruct
}  // namespace test
