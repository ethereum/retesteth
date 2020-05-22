#include "EthGetBlockByTransaction.h"
#include <retesteth/EthChecks.h>

namespace test
{
namespace teststruct
{
EthGetBlockByTransaction::EthGetBlockByTransaction(DataObject const& _data)
{
    try
    {
        if (_data.getSubObjects().size() == 0)
            m_hash = spFH32(new FH32(_data));
        else
        {
            m_blockHash = spFH32(new FH32(_data.atKey("blockHash")));
            m_blockNumber = spVALUE(new VALUE(_data.atKey("blockNumber")));
            m_from = spFH20(new FH20(_data.atKey("from")));
            m_gas = spVALUE(new VALUE(_data.atKey("gas")));
            m_gasPrice = spVALUE(new VALUE(_data.atKey("gasPrice")));
            m_hash = spFH32(new FH32(_data.atKey("hash")));
            m_input = spBYTES(new BYTES(_data.atKey("input")));
            m_nonce = spVALUE(new VALUE(_data.atKey("nonce")));
            m_to = spFH20(new FH20(_data.atKey("to")));
            m_transactionIndex = spVALUE(new VALUE(_data.atKey("transactionIndex")));
            m_value = spVALUE(new VALUE(_data.atKey("value")));
            m_v = spVALUE(new VALUE(_data.atKey("v"), dev::u256("0xff")));
            m_r = spVALUE(new VALUE(_data.atKey("r")));
            m_s = spVALUE(new VALUE(_data.atKey("s")));
        }
    }
    catch (std::exception const& _ex)
    {
        ETH_ERROR_MESSAGE(string("EthGetBlockByTransaction unmarshal error: ") + _ex.what());
    }
}

}  // namespace teststruct
}  // namespace test
