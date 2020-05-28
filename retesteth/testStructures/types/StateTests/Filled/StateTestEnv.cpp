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

        // Indicates first block timestamp in StateTests
        m_currentTimestamp = spVALUE(new VALUE(_data.atKey("currentTimestamp")));
        // Indicates zero block timestamp in StateTests
        m_genesisTimestamp = spVALUE(new VALUE(0));

        m_previousHash = spFH32(new FH32(_data.atKey("previousHash")));

        DataObject tmpD;
        tmpD = "0x00";  // State Tests extra data is 0x00
        m_currentExtraData = spBYTES(new BYTES(tmpD));
        m_currentNonce = spFH8(new FH8(FH8::zero()));
        m_currentMixHash = spFH32(new FH32(FH32::zero()));
    }
    catch (DataObjectException const& _ex)
    {
        ETH_ERROR_MESSAGE(_ex.what());
    }
}

}  // namespace teststruct
}  // namespace test
