#include "StateTestTransaction.h"
#include <retesteth/TestHelper.h>
#include <retesteth/testStructures/Common.h>

namespace test
{
namespace teststruct
{
StateTestTransaction::StateTestTransaction(DataObject const& _data)
{
    try
    {
        if (_data.atKey("to").asString().empty())
            m_creation = true;
        else
        {
            m_creation = false;
            m_to = spFH20(new FH20(_data.atKey("to")));
        }
        m_secretKey = spFH32(new FH32(_data.atKey("secretKey")));
        m_gasPrice = spVALUE(new VALUE(_data.atKey("gasPrice")));
        m_nonce = spVALUE(new VALUE(_data.atKey("nonce")));

        std::map<string, spAccessList> accessLists;
        if (_data.count("accessLists"))
        {
            for (auto const& el : _data.atKey("accessLists").getSubObjects())
                accessLists.emplace(el.getKey(), spAccessList(new AccessList(el)));
        }

        size_t index = 0;
        for (auto const& el : _data.atKey("data").getSubObjects())
        {
            DataObject dataInKey = el;
            string const indexs = test::fto_string(index);
            if (accessLists.count(indexs))
                m_databox.push_back(Databox(dataInKey, dataInKey.asString(), string(), accessLists.at(indexs)));
            else
                m_databox.push_back(Databox(dataInKey, dataInKey.asString(), string()));
            index++;
        }
        for (auto const& el : _data.atKey("gasLimit").getSubObjects())
            m_gasLimit.push_back(el);
        for (auto const& el : _data.atKey("value").getSubObjects())
            m_value.push_back(el);

        requireJsonFields(_data, "StateTestTransaction " + _data.getKey(),
            {{"data", {{DataType::Array}, jsonField::Required}},
             {"accessLists", {{DataType::Object}, jsonField::Optional}},
             {"gasLimit", {{DataType::Array}, jsonField::Required}},
             {"gasPrice", {{DataType::String}, jsonField::Required}},
             {"nonce", {{DataType::String}, jsonField::Required}},
             {"value", {{DataType::Array}, jsonField::Required}},
             {"to", {{DataType::String}, jsonField::Required}},
             {"secretKey", {{DataType::String}, jsonField::Required}}});
    }
    catch (std::exception const& _ex)
    {
        throw UpwardsException(string("StateTestTransaction parse error: ") + _ex.what() + _data.asJson());
    }
}

}  // namespace teststruct
}  // namespace test
