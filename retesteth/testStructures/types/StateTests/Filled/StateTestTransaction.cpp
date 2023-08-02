#include "StateTestTransaction.h"
#include <retesteth/EthChecks.h>
#include <retesteth/testStructures/Common.h>
#include <retesteth/Constants.h>

using namespace std;
using namespace test::teststruct;
using namespace test::teststruct::constnames;

namespace  {
void require1559TransactionScheme(DataObject const& _data)
{
    REQUIRE_JSONFIELDS(_data, "StateTestTransaction " + _data.getKey(),
        {{c_data, {{DataType::Array}, jsonField::Required}},
            {c_accessLists, {{DataType::Array}, jsonField::Required}},
            {c_gasLimit, {{DataType::Array}, jsonField::Required}},
            {c_nonce, {{DataType::String}, jsonField::Required}},
            {c_value, {{DataType::Array}, jsonField::Required}},
            {c_to, {{DataType::String}, jsonField::Required}},
            {c_maxFeePerGas, {{DataType::String}, jsonField::Required}},
            {c_maxPriorityFeePerGas, {{DataType::String}, jsonField::Required}},
            {c_secretKey, {{DataType::String}, jsonField::Required}},
            {c_sender, {{DataType::String}, jsonField::Optional}}
        });
}

void requireBlobTransactionScheme(DataObject const& _data)
{
    REQUIRE_JSONFIELDS(_data, "StateTestTransaction " + _data.getKey(),
        {{c_data, {{DataType::Array}, jsonField::Required}},
            {c_accessLists, {{DataType::Array}, jsonField::Required}},
            {c_gasLimit, {{DataType::Array}, jsonField::Required}},
            {c_nonce, {{DataType::String}, jsonField::Required}},
            {c_value, {{DataType::Array}, jsonField::Required}},
            {c_to, {{DataType::String}, jsonField::Required}},
            {c_maxFeePerGas, {{DataType::String}, jsonField::Required}},
            {c_maxPriorityFeePerGas, {{DataType::String}, jsonField::Required}},
            {c_maxFeePerBlobGas, {{DataType::String}, jsonField::Required}},
            {c_blobVersionedHashes, {{DataType::Array}, jsonField::Required}},
            {c_secretKey, {{DataType::String}, jsonField::Required}},
            {c_sender, {{DataType::String}, jsonField::Optional}}
        });
}

void requireLegacyAccessListTransactionScheme(DataObject const& _data)
{
    REQUIRE_JSONFIELDS(_data, "StateTestTransaction " + _data.getKey(),
        {{c_data, {{DataType::Array}, jsonField::Required}},
            {c_accessLists, {{DataType::Array}, jsonField::Optional}},
            {c_gasLimit, {{DataType::Array}, jsonField::Required}},
            {c_gasPrice, {{DataType::String}, jsonField::Required}},
            {c_nonce, {{DataType::String}, jsonField::Required}},
            {c_value, {{DataType::Array}, jsonField::Required}},
            {c_to, {{DataType::String}, jsonField::Required}},
            {c_secretKey, {{DataType::String}, jsonField::Required}},
            {c_sender, {{DataType::String}, jsonField::Optional}}
        });
}
}

namespace test::teststruct
{
StateTestTransaction::StateTestTransaction(DataObject const& _data)
{
    try
    {
        if (_data.count(c_maxFeePerGas) || _data.count(c_maxPriorityFeePerGas))
        {
            if (_data.count(c_maxFeePerBlobGas) || _data.count(c_blobVersionedHashes))
            {
                requireBlobTransactionScheme(_data);
                m_maxFeePerBlobGas = sVALUE(_data.atKey(c_maxFeePerBlobGas));
                for (auto const& el : _data.atKey(c_blobVersionedHashes).getSubObjects())
                    m_blobVersionedHashes.emplace_back(FH32(el));

                m_maxFeePerGas = sVALUE(_data.atKey(c_maxFeePerGas));
                m_maxPriorityFeePerGas = sVALUE(_data.atKey(c_maxPriorityFeePerGas));
            }
            else
            {
                require1559TransactionScheme(_data);
                m_maxFeePerGas = sVALUE(_data.atKey(c_maxFeePerGas));
                m_maxPriorityFeePerGas = sVALUE(_data.atKey(c_maxPriorityFeePerGas));
            }
        }
        else
        {
            requireLegacyAccessListTransactionScheme(_data);
            m_gasPrice = sVALUE(_data.atKey(c_gasPrice));
        }

        if (_data.atKey(c_to).asString().empty())
            m_creation = true;
        else
        {
            m_creation = false;
            m_to = sFH20(_data.atKey(c_to));
        }
        m_secretKey = sFH32(_data.atKey(c_secretKey));
        m_nonce = sVALUE(_data.atKey(c_nonce));

        std::vector<spAccessList> accessLists;
        if (_data.count(c_accessLists))
        {
            for (auto const& el : _data.atKey(c_accessLists).getSubObjects())
            {
                if (el->type() == DataType::Null)
                    accessLists.emplace_back(spAccessList(0));
                else
                    accessLists.emplace_back(spAccessList(new AccessList(el)));
            }
        }

        size_t index = 0;
        if (_data.count(c_accessLists))
            if (accessLists.size() != _data.atKey(c_data).getSubObjects().size())
                ETH_ERROR_MESSAGE("`AccessLists` array length must match `data` array length!");

        m_databox.reserve(_data.atKey(c_data).getSubObjects().size());
        for (auto const& el : _data.atKey(c_data).getSubObjects())
        {
            BYTES dataInKey = BYTES(el.getCContent());
            string const sDataPreview = el->asString().substr(0, 8);
            if (accessLists.size())
            {
                if (accessLists.at(index).isEmpty())
                    m_databox.emplace_back(Databox(dataInKey, string(), sDataPreview));
                else
                    m_databox.emplace_back(Databox(dataInKey, string(), sDataPreview, accessLists.at(index)));
            }
            else
                m_databox.emplace_back(Databox(dataInKey, string(), sDataPreview));

            index++;
        }

        for (auto const& el : _data.atKey(c_gasLimit).getSubObjects())
            m_gasLimit.emplace_back(el.getCContent());
        for (auto const& el : _data.atKey(c_value).getSubObjects())
            m_value.emplace_back(el.getCContent());
    }
    catch (std::exception const& _ex)
    {
        throw UpwardsException(string("StateTestTransaction parse error: ") + _ex.what() + _data.asJson());
    }
}

}  // namespace teststruct
