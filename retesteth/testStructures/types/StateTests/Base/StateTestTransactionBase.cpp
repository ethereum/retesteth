#include "StateTestTransactionBase.h"
#include <retesteth/ExitHandler.h>

using namespace dataobject;
using namespace test::teststruct;

spDataObject StateTestTransactionBase::asDataObject() const
{
    // Serialize data back to JSON
    spDataObject _out(new DataObject());
    DataObject& out = _out.getContent();
    size_t index = 0;
    bool atLeastOneNonNullAccessList = false;
    spDataObject txAccessListData(new DataObject(DataType::Array));
    for (Databox const& el : m_databox)
    {
        spDataObject elb(new DataObject(el.m_data.asString()));
        out["data"].addArrayObject(elb);
        if (el.m_accessList.isEmpty())
            (*txAccessListData).addArrayObject(spDataObject(new DataObject(DataType::Null)));
        else
        {
            (*txAccessListData).addArrayObject(el.m_accessList->asDataObject());
            atLeastOneNonNullAccessList = true;
        }
        index++;
    }

    if (atLeastOneNonNullAccessList)
        out.atKeyPointer("accessLists") = txAccessListData;

    if (!m_maxFeePerGas.isEmpty() || !m_maxPriorityFeePerGas.isEmpty())
    {
        if (m_maxFeePerGas.isEmpty() || m_maxPriorityFeePerGas.isEmpty())
            ETH_FAIL_MESSAGE("Must be defined both m_maxFeePerGas and m_maxPriorityFeePerGas!");
        if (!atLeastOneNonNullAccessList)
            ETH_FAIL_MESSAGE("Basefee transaction must have accesslist!");
        out["maxFeePerGas"] = m_maxFeePerGas->asString();
        out["maxPriorityFeePerGas"] = m_maxPriorityFeePerGas->asString();
    }
    else
    {
        // Legacy Transaction
        out["gasPrice"] = m_gasPrice->asString();
    }

    for (VALUE const& el : m_gasLimit)
    {
        spDataObject els(new DataObject(el.asString()));
        out["gasLimit"].addArrayObject(els);
    }
    out["nonce"] = m_nonce->asString();
    out["secretKey"] = m_secretKey->asString();
    if (m_creation)
        out["to"] = "";
    else
        out["to"] = m_to->asString();
    for (VALUE const& el : m_value)
    {
        spDataObject els(new DataObject(el.asString()));
        out["value"].addArrayObject(els);
    }

    return _out;
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
                trData["nonce"] = m_nonce->asString();
                if (m_creation)
                    trData["to"] = "";
                else
                    trData["to"] = m_to->asString();
                trData["secretKey"] = m_secretKey->asString();

                // EIP 1559
                if (!m_maxPriorityFeePerGas.isEmpty())
                {
                    // Type 0x02 transaction fields
                    trData["maxPriorityFeePerGas"] = m_maxPriorityFeePerGas->asString();
                    trData["maxFeePerGas"] = m_maxFeePerGas->asString();

                    if (databox.m_accessList.isEmpty())
                        ETH_FAIL_MESSAGE("BaseFeeTransaction must have access list!");
                }
                else
                {
                    trData["gasPrice"] = m_gasPrice->asString();
                }

                // Export Access List
                if (!databox.m_accessList.isEmpty())
                    trData.atKeyPointer("accessList") = databox.m_accessList->asDataObject();

                out.push_back(
                    TransactionInGeneralSection(trData, dIND, gIND, vIND, databox.m_dataRawPreview, databox.m_dataLabel));
            }
        }
    }
    return out;
}
