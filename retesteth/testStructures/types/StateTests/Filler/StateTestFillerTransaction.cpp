#include "StateTestFillerTransaction.h"
#include <retesteth/EthChecks.h>
#include <retesteth/TestHelper.h>
#include <retesteth/testStructures/Common.h>

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
            m_creation = true;
        else
        {
            m_creation = false;
            DataObject dTo = _data.atKey("to");
            string const& to = _data.atKey("to").asString();
            if (to.size() > 1 && to[1] != 'x')
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

            string label;
            string rawData = dataInKey.asString();
            std::string const c_labelPrefix = ":label";
            if (rawData.find(c_labelPrefix) != string::npos)
            {
                size_t const pos = rawData.find(c_labelPrefix);
                size_t const posEnd = rawData.find(' ', pos + c_labelPrefix.size() + 1);
                if (posEnd != string::npos)
                {
                    label = rawData.substr(pos, posEnd - pos);
                    rawData = rawData.substr(posEnd + 1);  // remove label before code parsing
                }
                else
                {
                    label = rawData.substr(pos);
                    rawData = "";
                }
            }
            m_dataLabel.push_back(label);
            dataInKey.setString(test::compiler::replaceCode(rawData));
            // ---
            m_data.push_back(dataInKey);
        }
        for (auto const& el : tmpD.atKey("gasLimit").getSubObjects())
            m_gasLimit.push_back(el);
        for (auto const& el : tmpD.atKey("value").getSubObjects())
            m_value.push_back(el);

        requireJsonFields(_data, "StateTestFillerTransaction " + _data.getKey(),
            {{"data", {{DataType::Array}, jsonField::Required}},
             {"gasLimit", {{DataType::Array}, jsonField::Required}},
             {"gasPrice", {{DataType::String}, jsonField::Required}},
             {"nonce", {{DataType::String}, jsonField::Required}},
             {"value", {{DataType::Array}, jsonField::Required}},
             {"to", {{DataType::String}, jsonField::Required}},
             {"secretKey", {{DataType::String}, jsonField::Required}}});
    }
    catch (std::exception const& _ex)
    {
        throw UpwardsException(string("StateTestFillerTransaction parse error: ") + _ex.what() + "\n" + _data.asJson());
    }
}

}  // namespace teststruct
}  // namespace test
