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

        // Indicates first block timestamp in StateTests
        m_currentTimestamp = spVALUE(new VALUE(tmpData.atKey("currentTimestamp")));
        // Indicates zero block timestamp in StateTests
        m_genesisTimestamp = spVALUE(new VALUE(0));

        // Do not allow hash without 0x
        m_previousHash = spFH32(new FH32(_data.atKey("previousHash")));

        DataObject tmpD;
        tmpD = "0x00";  // State Tests extra data is 0x00
        m_currentExtraData = spBYTES(new BYTES(tmpD));
        m_currentNonce = spFH8(new FH8(FH8::zero()));
        m_currentMixHash = spFH32(new FH32(FH32::zero()));
    }
    catch (std::exception const& _ex)
    {
        throw BaseEthException(string("StateTestFillerEnv parse error: ") + _ex.what());
    }
}

}  // namespace teststruct
}  // namespace test
