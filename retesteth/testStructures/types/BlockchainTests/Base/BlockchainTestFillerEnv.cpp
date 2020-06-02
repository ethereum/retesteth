#include "BlockchainTestFillerEnv.h"
#include <retesteth/EthChecks.h>
#include <retesteth/ethObjects/object.h>
namespace test
{
namespace teststruct
{
// Env. Build a type representation
BlockchainTestFillerEnv::BlockchainTestFillerEnv(DataObject const& _data)
{
    try
    {
        DataObject tmpData = _data;
        tmpData.performModifier(mod_valueToCompactEvenHexPrefixed);
        m_currentCoinbase = spFH20(new FH20(tmpData.atKey("coinbase")));
        m_currentDifficulty = spVALUE(new VALUE(tmpData.atKey("difficulty")));
        m_currentGasLimit = spVALUE(new VALUE(tmpData.atKey("gasLimit"), dev::u256("0x7fffffffffffffff")));
        m_currentNumber = spVALUE(new VALUE(tmpData.atKey("number")));
        m_currentTimestamp = spVALUE(new VALUE(tmpData.atKey("timestamp")));
        m_previousHash = spFH32(new FH32(tmpData.atKey("parentHash")));

        m_currentExtraData = spBYTES(new BYTES(tmpData.atKey("extraData")));
        m_currentNonce = spFH8(new FH8(tmpData.atKey("nonce")));
        m_currentMixHash = spFH32(new FH32(tmpData.atKey("mixHash")));
    }
    catch (DataObjectException const& _ex)
    {
        ETH_ERROR_MESSAGE(_ex.what());
    }
}

}  // namespace teststruct
}  // namespace test
