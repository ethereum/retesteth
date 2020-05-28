#include "StateTestFillerTransaction.h"
#include <retesteth/EthChecks.h>
#include <retesteth/TestHelper.h>
#include <retesteth/ethObjects/object.h>

namespace test
{
namespace teststruct
{
StateTestFillerTransaction::StateTestFillerTransaction(DataObject const& _data)
{
    try
    {
        // Read data from JSON in test filler
        // The VALUE fields can be decimal -> convert it to hex
        // The data field can be LLL or other code -> compile it to BYTES
        if (_data.atKey("to").asString().empty())
        {
            m_creation = true;
        }
        else
        {
            m_creation = false;
            m_to = spFH20(new FH20(_data.atKey("to")));
        }
        m_secretKey = spFH32(new FH32(_data.atKey("secretKey")));
        m_gasPrice = spVALUE(new VALUE(_data.atKey("gasPrice")));
        m_nonce = spVALUE(new VALUE(_data.atKey("nonce")));

        for (auto const& el : _data.atKey("data").getSubObjects())
        {
            // -- Compile LLL in transaction data into byte code if not already
            DataObject dataInKey = el;
            dataInKey.setKey("`data` array element in General Transaction Section");  // Hint
            dataInKey.setString(test::replaceCode(dataInKey.asString()));
            // ---

            m_data.push_back(dataInKey);
        }
        for (auto const& el : _data.atKey("gasLimit").getSubObjects())
            m_gasLimit.push_back(el);
        for (auto const& el : _data.atKey("value").getSubObjects())
            m_value.push_back(el);
    }
    catch (std::exception const& _ex)
    {
        throw BaseEthException(string("StateTestFillerTransaction parse error: ") + _ex.what());
    }
}

}  // namespace teststruct
}  // namespace test
