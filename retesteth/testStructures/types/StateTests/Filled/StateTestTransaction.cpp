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

        for (auto const& el : _data.atKey("data").getSubObjects())
        {
            DataObject dataInKey;
            AccessList accessList;
            if (el.type() == DataType::Object)
            {
                dataInKey = el.atKey("data");
                accessList = AccessList(el.atKey("accessList"));
                requireJsonFields(el, "StateTestTransaction::dataWithList " + _data.getKey(),
                    {{"data", {{DataType::String}, jsonField::Required}},
                        {"accessList", {{DataType::Array}, jsonField::Required}}});
                m_databox.push_back(Databox(dataInKey, dataInKey.asString(), string(), accessList));
            }
            else
            {
                dataInKey = el;
                m_databox.push_back(Databox(dataInKey, dataInKey.asString(), string()));
            }
        }
        for (auto const& el : _data.atKey("gasLimit").getSubObjects())
            m_gasLimit.push_back(el);
        for (auto const& el : _data.atKey("value").getSubObjects())
            m_value.push_back(el);

        requireJsonFields(_data, "StateTestTransaction " + _data.getKey(),
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
        throw UpwardsException(string("StateTestTransaction parse error: ") + _ex.what() + _data.asJson());
    }
}

}  // namespace teststruct
}  // namespace test
