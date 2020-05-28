#include "BlockchainTestEnv.h"
#include <retesteth/EthChecks.h>
#include <retesteth/ethObjects/object.h>
namespace test
{
namespace teststruct
{
// Env. Build a type representation
BlockchainTestEnv::BlockchainTestEnv(DataObject const& _data)
{
    try
    {
        m_currentCoinbase = spFH20(new FH20(_data.atKey("coinbase")));
        m_currentDifficulty = spVALUE(new VALUE(_data.atKey("difficulty")));
        m_currentGasLimit = spVALUE(new VALUE(_data.atKey("gasLimit"), dev::u256("0x7fffffffffffffff")));
        m_currentNumber = spVALUE(new VALUE(_data.atKey("number")));
        m_currentTimestamp = spVALUE(new VALUE(_data.atKey("timestamp")));
        m_previousHash = spFH32(new FH32(_data.atKey("parentHash")));

        m_currentExtraData = spBYTES(new BYTES(_data.atKey("extraData")));
        m_currentNonce = spFH8(new FH8(_data.atKey("nonce")));
        m_currentMixHash = spFH32(new FH32(_data.atKey("mixHash")));
    }
    catch (DataObjectException const& _ex)
    {
        ETH_ERROR_MESSAGE(_ex.what());
    }
}

}  // namespace teststruct
}  // namespace test
