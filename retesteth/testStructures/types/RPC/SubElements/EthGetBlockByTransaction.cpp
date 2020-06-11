#include "EthGetBlockByTransaction.h"
#include <retesteth/EthChecks.h>
#include <retesteth/testStructures/Common.h>

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
            // Aleth return data field as "" instead of "0x"
            if (_data.atKey("data").asString().empty())
            {
                DataObject& _dataRef = const_cast<DataObject&>(_data);
                DataObject& _dataFieldRef = _dataRef.atKeyUnsafe("data");
                _dataFieldRef = "0x";
            }

            m_transaction = spTransaction(new Transaction(_data));
            m_blockHash = spFH32(new FH32(_data.atKey("blockHash")));
            m_blockNumber = spVALUE(new VALUE(_data.atKey("blockNumber")));
            m_from = spFH20(new FH20(_data.atKey("from")));
            m_hash = spFH32(new FH32(_data.atKey("hash")));
            m_transactionIndex = spVALUE(new VALUE(_data.atKey("transactionIndex")));
        }
    }
    catch (std::exception const& _ex)
    {
        ETH_ERROR_MESSAGE(string("EthGetBlockByTransaction unmarshal error: ") + _ex.what());
    }
}

}  // namespace teststruct
}  // namespace test
