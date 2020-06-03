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
        DataObject tmpD = _data;
        tmpD.removeKey("data");
        tmpD.performModifier(mod_valueToCompactEvenHexPrefixed);

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
            DataObject dTo = _data.atKey("to");
            if (_data.atKey("to").asString()[1] != 'x')
                dTo = "0x" + _data.atKey("to").asString();
            m_to = spFH20(new FH20(dTo));
        }
        m_secretKey = spFH32(new FH32(tmpD.atKey("secretKey")));
        m_gasPrice = spVALUE(new VALUE(tmpD.atKey("gasPrice")));
        m_nonce = spVALUE(new VALUE(tmpD.atKey("nonce")));

        for (auto const& el : _data.atKey("data").getSubObjects())
        {
            // -- Compile LLL in transaction data into byte code if not already
            DataObject dataInKey = el;
            dataInKey.setKey("`data` array element in General Transaction Section");  // Hint
            dataInKey.setString(test::replaceCode(dataInKey.asString()));
            // ---

            m_data.push_back(dataInKey);
        }
        for (auto const& el : tmpD.atKey("gasLimit").getSubObjects())
            m_gasLimit.push_back(el);
        for (auto const& el : tmpD.atKey("value").getSubObjects())
            m_value.push_back(el);
    }
    catch (std::exception const& _ex)
    {
        throw BaseEthException(string("StateTestFillerTransaction parse error: ") + _ex.what());
    }
}

}  // namespace teststruct
}  // namespace test
