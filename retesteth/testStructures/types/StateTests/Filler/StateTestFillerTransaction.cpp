#include "StateTestFillerTransaction.h"
#include <retesteth/EthChecks.h>
#include <retesteth/testStructures/Common.h>
#include <retesteth/Constants.h>

using namespace std;
using namespace dataobject;
using namespace test::teststruct;
using namespace test::teststruct::constnames;

namespace
{

void requireBlobTransactionScheme(spDataObject const& _data)
{
    REQUIRE_JSONFIELDS(_data, "StateTestFillerTransaction " + _data->getKey(),
        {{c_data, {{DataType::Array}, jsonField::Required}},
            {c_gasLimit, {{DataType::Array}, jsonField::Required}},
            {c_nonce, {{DataType::String}, jsonField::Required}},
            {c_value, {{DataType::Array}, jsonField::Required}},
            {c_to, {{DataType::String}, jsonField::Required}},
            {c_sender, {{DataType::String}, jsonField::Optional}},
            {c_maxFeePerGas, {{DataType::String}, jsonField::Required}},
            {c_maxPriorityFeePerGas, {{DataType::String}, jsonField::Required}},
            {c_maxFeePerBlobGas, {{DataType::String}, jsonField::Required}},
            {c_blobVersionedHashes, {{DataType::Array}, jsonField::Required}},
            {c_secretKey, {{DataType::String}, jsonField::Required}}});
}

void require1559TransactionScheme(spDataObject const& _data)
{
    REQUIRE_JSONFIELDS(_data, "StateTestFillerTransaction " + _data->getKey(),
        {{c_data, {{DataType::Array}, jsonField::Required}},
            {c_gasLimit, {{DataType::Array}, jsonField::Required}},
            {c_nonce, {{DataType::String}, jsonField::Required}},
            {c_value, {{DataType::Array}, jsonField::Required}},
            {c_to, {{DataType::String}, jsonField::Required}},
            {c_sender, {{DataType::String}, jsonField::Optional}},
            {c_maxFeePerGas, {{DataType::String}, jsonField::Required}},
            {c_maxPriorityFeePerGas, {{DataType::String}, jsonField::Required}},
            {c_secretKey, {{DataType::String}, jsonField::Required}}});
}

void requireLegacyTransctionScheme(spDataObject const& _data)
{
    // LEGACY TRANSACTION TEMPLATE (gtest FILLER)
    REQUIRE_JSONFIELDS(_data, "StateTestFillerTransaction " + _data->getKey(),
        {{c_data, {{DataType::Array}, jsonField::Required}},
            {c_gasLimit, {{DataType::Array}, jsonField::Required}},
            {c_gasPrice, {{DataType::String}, jsonField::Required}},
            {c_nonce, {{DataType::String}, jsonField::Required}},
            {c_value, {{DataType::Array}, jsonField::Required}},
            {c_to, {{DataType::String}, jsonField::Required}},
            {c_sender, {{DataType::String}, jsonField::Optional}},
            {c_secretKey, {{DataType::String}, jsonField::Required}}});
}
}

namespace test::teststruct
{
StateTestFillerTransaction::StateTestFillerTransaction(spDataObjectMove _data)
{
    try
    {
        m_rawData = _data.getPointer();
        if (m_rawData->count(c_maxFeePerBlobGas) || m_rawData->count(c_blobVersionedHashes))
            requireBlobTransactionScheme(m_rawData);
        else if (m_rawData->count(c_maxFeePerGas) || m_rawData->count(c_maxPriorityFeePerGas))
            require1559TransactionScheme(m_rawData);
        else
            requireLegacyTransctionScheme(m_rawData);

        // Read data from JSON in test filler
        // The VALUE fields can be decimal -> convert it to hex
        // The data field can be LLL or other code -> compile it to BYTES
        (*m_rawData).performModifier(mod_valueToCompactEvenHexPrefixed, DataObject::ModifierOption::RECURSIVE, {"data", "to", "secretKey"});
        if (m_rawData->count(c_secretKey))
            (*m_rawData).atKeyUnsafe(c_secretKey).performModifier(mod_valueInsertZeroXPrefix);

        if (m_rawData->atKey(c_to).asString().empty())
            m_creation = true;
        else
        {
            m_creation = false;
            (*m_rawData).atKeyUnsafe(c_to).performModifier(mod_valueInsertZeroXPrefix);
            m_to = sFH20(m_rawData->atKey(c_to));
        }

        m_secretKey = sFH32(m_rawData->atKey(c_secretKey));
        m_publicKey = convertSecretToPublic(m_secretKey);
        (*m_rawData)[c_sender] = m_publicKey->asString();

        m_nonce = sVALUE(m_rawData->atKey(c_nonce));
        m_databox.reserve(m_rawData->atKey(c_data).getSubObjects().size());
        for (auto& dataEl : (*m_rawData).atKeyUnsafe(c_data).getSubObjectsUnsafe())
        {
            spAccessList accessList;
            spDataObject actualDataField;
            if (dataEl->type() == DataType::Object)
            {
                REQUIRE_JSONFIELDS(dataEl, "StateTestFillerTransaction::dataWithList " + m_rawData->getKey(),
                    {{c_data, {{DataType::String}, jsonField::Required}},
                     {c_accessList, {{DataType::Array}, jsonField::Required}}});

                actualDataField = (*dataEl).atKeyPointerUnsafe(c_data);
                accessList = spAccessList(new AccessList(dataEl->atKey(c_accessList)));
            }
            else
                actualDataField = dataEl;

            // -- Compile LLL in transaction data into byte code if not already
            // Detect :label prefix over code compilation
            string label;
            string rawData = actualDataField->asString();
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
            (*actualDataField).setString(test::compiler::replaceCode(rawData));
            // ---
            m_databox.emplace_back(Databox(BYTES(actualDataField.getContent()), label, rawData.substr(0, 30), accessList));
        }
        for (auto const& el : m_rawData->atKey(c_gasLimit).getSubObjects())
            m_gasLimit.emplace_back(el.getCContent());
        for (auto const& el : m_rawData->atKey(c_value).getSubObjects())
            m_value.emplace_back(el.getCContent());

        if (m_rawData->count(c_maxFeePerGas) || m_rawData->count(c_maxPriorityFeePerGas))
        {
            // EIP 1559 TRANSACTION TEMPLATE (gtest FILLER)
            m_maxFeePerGas = sVALUE(m_rawData->atKey(c_maxFeePerGas));
            m_maxPriorityFeePerGas = sVALUE(m_rawData->atKey(c_maxPriorityFeePerGas));
        }
        else
        {
            // LEGACY TRANSACTION TEMPLATE (gtest FILLER)
            m_gasPrice = sVALUE(m_rawData->atKey(c_gasPrice));
        }

        if (m_rawData->count(c_blobVersionedHashes) || m_rawData->count(c_maxFeePerBlobGas))
        {
            for (auto const& el : m_rawData->atKey(c_blobVersionedHashes).getSubObjects())
                m_blobVersionedHashes.emplace_back(FH32(el));
            m_maxFeePerBlobGas = sVALUE(m_rawData->atKey(c_maxFeePerBlobGas));
        }


        // Prepare data for export (export format is different to import format in this case
        // This is done as part of optimization. So not to construct export every time on request
        // Export data (m_rawData) is prepared in constructor and then promise that
        // it corresponds to the actual data in the class (class does not change after parsing)
        (*m_rawData).performModifier(mod_valueToLowerCase);
        bool atLeastOneNonNullAccessList = false;
        spDataObject exportDatas;
        spDataObject txAccessListData(new DataObject(DataType::Array));
        for (Databox const& el : m_databox)
        {
            spDataObject elb(new DataObject(el.m_data.asString()));
            (*exportDatas).addArrayObject(elb);
            if (el.m_accessList.isEmpty())
                (*txAccessListData).addArrayObject(sDataObject(DataType::Null));
            else
            {
                (*txAccessListData).addArrayObject(el.m_accessList->asDataObject());
                atLeastOneNonNullAccessList = true;
            }
        }

        (*exportDatas).setKey(c_data);
        (*m_rawData).atKeyPointer(c_data) = exportDatas;
        if (atLeastOneNonNullAccessList)
            (*m_rawData).atKeyPointer(c_accessLists) = txAccessListData;

        if (!m_maxFeePerGas.isEmpty() || !m_maxPriorityFeePerGas.isEmpty())
        {
            if (m_maxFeePerGas.isEmpty() || m_maxPriorityFeePerGas.isEmpty())
                ETH_FAIL_MESSAGE("Must be defined both m_maxFeePerGas and m_maxPriorityFeePerGas!");
            if (!atLeastOneNonNullAccessList)
                ETH_FAIL_MESSAGE("Basefee transaction must have accesslist!");
        }
    }
    catch (std::exception const& _ex)
    {
        throw UpwardsException(string("StateTestFillerTransaction parse error: ") + _ex.what() + "\n" + m_rawData->asJson());
    }
}

}  // namespace teststruct
