#include "StateTestEnvBase.h"
namespace test::teststruct
{

spDataObject StateTestEnvBase::asDataObject() const
{
    spDataObject data;
    (*data)["currentCoinbase"] = m_currentCoinbase->asString();
    (*data)["currentGasLimit"] = m_currentGasLimit->asString();
    (*data)["currentNumber"] = m_currentNumber->asString();
    (*data)["currentTimestamp"] = m_currentTimestamp->asString();
    (*data)["currentRandom"] = m_currentRandom->asString();
    (*data)["currentDifficulty"] = m_currentDifficulty->asString();
    (*data)["currentBaseFee"] = m_currentBaseFee->asString();
    (*data)["currentExcessBlobGas"] = m_currentExcessBlobGas->asString();

    //(*data)["currentRequestsHash"] = m_currentRequestsHash->asString();
    return data;
}

}  // namespace teststruct
