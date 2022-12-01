#include "EthGetBlockByTransaction.h"
#include <retesteth/EthChecks.h>
#include <retesteth/testStructures/Common.h>

using namespace std;
namespace test::teststruct
{
EthGetBlockByTransaction::EthGetBlockByTransaction(spDataObjectMove _data)
{
    try
    {
        spDataObject data = _data.getPointer();
        if (data->getSubObjects().size() == 0)
            m_hash = spFH32(new FH32(_data));
        else
        {
            // Aleth return data field as "" instead of "0x"
            if (data->atKey("data").asString().empty())
                (*data)["data"] = "0x";

            m_blockHash = spFH32(new FH32(data->atKey("blockHash")));
            m_blockNumber = spVALUE(new VALUE(data->atKey("blockNumber")));
            m_from = spFH20(new FH20(data->atKey("from")));
            m_hash = spFH32(new FH32(data->atKey("hash")));
            m_transactionIndex = spVALUE(new VALUE(data->atKey("transactionIndex")));

            // Construct transaction from json rpc request
            m_transaction = readTransaction(_data);
        }
    }
    catch (std::exception const& _ex)
    {
        ETH_ERROR_MESSAGE(string("EthGetBlockByTransaction unmarshal error: ") + _ex.what());
    }
}

}  // namespace teststruct
