#include "StateTestTransaction.h"
#include <retesteth/TestHelper.h>
#include <retesteth/testStructures/Common.h>

using namespace std;
namespace test::teststruct
{
StateTestTransaction::StateTestTransaction(DataObject const& _data)
{
    try
    {
        if (_data.count("maxFeePerGas") || _data.count("maxPriorityFeePerGas"))
        {
            REQUIRE_JSONFIELDS(_data, "StateTestTransaction " + _data.getKey(),
                {{"data", {{DataType::Array}, jsonField::Required}},
                    {"accessLists", {{DataType::Array}, jsonField::Required}},
                    {"gasLimit", {{DataType::Array}, jsonField::Required}},
                    {"nonce", {{DataType::String}, jsonField::Required}},
                    {"value", {{DataType::Array}, jsonField::Required}},
                    {"to", {{DataType::String}, jsonField::Required}},
                    {"maxFeePerGas", {{DataType::String}, jsonField::Required}},
                    {"maxPriorityFeePerGas", {{DataType::String}, jsonField::Required}},
                    {"secretKey", {{DataType::String}, jsonField::Required}},
                    {"sender", {{DataType::String}, jsonField::Optional}}
                });
            // EIP 1559 style transaction
            m_maxFeePerGas = spVALUE(new VALUE(_data.atKey("maxFeePerGas")));
            m_maxPriorityFeePerGas = spVALUE(new VALUE(_data.atKey("maxPriorityFeePerGas")));

        }
        else
        {
            REQUIRE_JSONFIELDS(_data, "StateTestTransaction " + _data.getKey(),
                {{"data", {{DataType::Array}, jsonField::Required}},
                    {"accessLists", {{DataType::Array}, jsonField::Optional}},
                    {"gasLimit", {{DataType::Array}, jsonField::Required}},
                    {"gasPrice", {{DataType::String}, jsonField::Required}},
                    {"nonce", {{DataType::String}, jsonField::Required}},
                    {"value", {{DataType::Array}, jsonField::Required}},
                    {"to", {{DataType::String}, jsonField::Required}},
                    {"secretKey", {{DataType::String}, jsonField::Required}},
                    {"sender", {{DataType::String}, jsonField::Optional}}
                });
            // Legacy and access list style transaction
            m_gasPrice = spVALUE(new VALUE(_data.atKey("gasPrice")));
        }

        if (_data.atKey("to").asString().empty())
            m_creation = true;
        else
        {
            m_creation = false;
            m_to = spFH20(new FH20(_data.atKey("to")));
        }
        m_secretKey = spFH32(new FH32(_data.atKey("secretKey")));
        m_nonce = spVALUE(new VALUE(_data.atKey("nonce")));

        std::vector<spAccessList> accessLists;
        if (_data.count("accessLists"))
        {
            for (auto const& el : _data.atKey("accessLists").getSubObjects())
            {
                if (el->type() == DataType::Null)
                    accessLists.push_back(spAccessList(0));
                else
                    accessLists.push_back(spAccessList(new AccessList(el)));
            }
        }

        size_t index = 0;
        if (_data.count("accessLists"))
            if (accessLists.size() != _data.atKey("data").getSubObjects().size())
                ETH_ERROR_MESSAGE("`AccessLists` array length must match `data` array length!");

        for (auto const& el : _data.atKey("data").getSubObjects())
        {
            BYTES dataInKey = BYTES(el.getCContent());
            string const sDataPreview = el->asString().substr(0, 8);
            if (accessLists.size())
            {
                if (accessLists.at(index).isEmpty())
                    m_databox.push_back(Databox(dataInKey, string(), sDataPreview));
                else
                    m_databox.push_back(Databox(dataInKey, string(), sDataPreview, accessLists.at(index)));
            }
            else
                m_databox.push_back(Databox(dataInKey, string(), sDataPreview));

            index++;
        }
        for (auto const& el : _data.atKey("gasLimit").getSubObjects())
            m_gasLimit.push_back(el.getCContent());
        for (auto const& el : _data.atKey("value").getSubObjects())
            m_value.push_back(el.getCContent());
    }
    catch (std::exception const& _ex)
    {
        throw UpwardsException(string("StateTestTransaction parse error: ") + _ex.what() + _data.asJson());
    }
}

}  // namespace teststruct
