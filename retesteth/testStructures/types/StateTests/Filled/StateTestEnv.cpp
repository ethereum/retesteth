#include "StateTestEnv.h"
#include <retesteth/EthChecks.h>
#include <retesteth/ethObjects/object.h>
namespace test
{
namespace teststruct
{
// Env. Build a type representation
StateTestEnv::StateTestEnv(DataObject const& _data)
{
    try
    {
        m_currentCoinbase = spFH20(new FH20(_data.atKey("currentCoinbase")));
        m_currentDifficulty = spVALUE(new VALUE(_data.atKey("currentDifficulty")));
        m_currentGasLimit = spVALUE(new VALUE(_data.atKey("currentGasLimit"), dev::u256("0x7fffffffffffffff")));
        m_currentNumber = spVALUE(new VALUE(_data.atKey("currentNumber")));
        m_currentTimestamp = spVALUE(new VALUE(_data.atKey("currentTimestamp")));
        m_previousHash = spFH32(new FH32(_data.atKey("previousHash")));
    }
    catch (DataObjectException const& _ex)
    {
        ETH_ERROR_MESSAGE(_ex.what());
    }
}

}  // namespace teststruct
}  // namespace test
