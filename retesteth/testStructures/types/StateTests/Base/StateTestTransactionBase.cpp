#include "StateTestTransactionBase.h"
#include <retesteth/ExitHandler.h>

using namespace dataobject;
using namespace test::teststruct;

const DataObject StateTestTransactionBase::asDataObject() const
{
    // Serialize data back to JSON
    DataObject out;
    size_t index = 0;
    bool atLeastOneNonNullAccessList = false;
    DataObject txAccessListData(DataType::Array);
    for (Databox const& el : m_databox)
    {
        out["data"].addArrayObject(el.m_data.asString());
        if (el.m_accessList.isEmpty())
            txAccessListData.addArrayObject(DataObject(DataType::Null));
        else
        {
            txAccessListData.addArrayObject(el.m_accessList.getCContent().asDataObject());
            atLeastOneNonNullAccessList = true;
        }
        index++;
    }

    if (atLeastOneNonNullAccessList)
        out["accessLists"] = txAccessListData;

    if (!m_maxFeePerGas.isEmpty() || !m_maxInclusionFeePerGas.isEmpty())
    {
        if (m_maxFeePerGas.isEmpty() || m_maxInclusionFeePerGas.isEmpty())
            ETH_FAIL_MESSAGE("Must be defined both m_maxFeePerGas and m_maxInclusionFeePerGas!");
        if (!atLeastOneNonNullAccessList)
            ETH_FAIL_MESSAGE("Basefee transaction must have accesslist!");
        out["maxFeePerGas"] = m_maxFeePerGas.getCContent().asString();
        out["maxInclusionFeePerGas"] = m_maxInclusionFeePerGas.getCContent().asString();
    }
    else
    {
        // Legacy Transaction
        out["gasPrice"] = m_gasPrice.getCContent().asString();
    }

    for (VALUE const& el : m_gasLimit)
        out["gasLimit"].addArrayObject(el.asString());
    out["nonce"] = m_nonce.getCContent().asString();
    out["secretKey"] = m_secretKey.getCContent().asString();
    if (m_creation)
        out["to"] = "";
    else
        out["to"] = m_to.getCContent().asString();
    for (VALUE const& el : m_value)
        out["value"].addArrayObject(el.asString());

    return out;
}

/// Construct individual transactions from gstate test transaction
std::vector<TransactionInGeneralSection> StateTestTransactionBase::buildTransactions() const
{
    // Construct vector of all transactions that are described int data
    std::vector<TransactionInGeneralSection> out;
    for (size_t dIND = 0; dIND < m_databox.size(); dIND++)
    {
        for (size_t gIND = 0; gIND < m_gasLimit.size(); gIND++)
        {
            for (size_t vIND = 0; vIND < m_value.size(); vIND++)
            {
                Databox const& databox = m_databox.at(dIND);
                if (ExitHandler::receivedExitSignal())
                    return out;
                DataObject trData;
                trData["data"] = databox.m_data.asString();
                trData["gasLimit"] = m_gasLimit.at(gIND).asString();
                trData["value"] = m_value.at(vIND).asString();
                trData["nonce"] = m_nonce.getCContent().asString();
                if (m_creation)
                    trData["to"] = "";
                else
                    trData["to"] = m_to.getCContent().asString();
                trData["secretKey"] = m_secretKey.getCContent().asString();

                // EIP 1559
                if (!m_maxInclusionFeePerGas.isEmpty())
                {
                    // Type 0x02 transaction fields
                    trData["maxInclusionFeePerGas"] = m_maxInclusionFeePerGas.getCContent().asString();
                    trData["maxFeePerGas"] = m_maxFeePerGas.getCContent().asString();

                    if (databox.m_accessList.isEmpty())
                        ETH_FAIL_MESSAGE("BaseFeeTransaction must have access list!");
                }
                else
                {
                    trData["gasPrice"] = m_gasPrice.getCContent().asString();
                }

                // Export Access List
                if (!databox.m_accessList.isEmpty())
                    trData["accessList"] = databox.m_accessList.getCContent().asDataObject();

                out.push_back(
                    TransactionInGeneralSection(trData, dIND, gIND, vIND, databox.m_dataRawPreview, databox.m_dataLabel));
            }
        }
    }
    return out;
}
