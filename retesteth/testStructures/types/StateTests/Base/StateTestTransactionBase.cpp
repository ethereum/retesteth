#include "StateTestTransactionBase.h"
#include <retesteth/ExitHandler.h>

using namespace dataobject;
using namespace test::teststruct;

const DataObject StateTestTransactionBase::asDataObject() const
{
    // Serialize data back to JSON
    DataObject out;
    size_t index = 0;
    DataObject txAccessListData;
    for (Databox const& el : m_databox)
    {
        out["data"].addArrayObject(el.m_data.asString());
        if (!el.m_accessList.isEmpty() && el.m_accessList.getCContent().list().size() != 0)
            txAccessListData[test::fto_string(index)] = el.m_accessList.getCContent().asDataObject();
        index++;
    }

    if (txAccessListData.getSubObjects().size())
        out["accessLists"] = txAccessListData;

    for (VALUE const& el : m_gasLimit)
        out["gasLimit"].addArrayObject(el.asString());
    out["gasPrice"] = m_gasPrice.getCContent().asString();
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

                trData["gasPrice"] = m_gasPrice.getCContent().asString();
                trData["nonce"] = m_nonce.getCContent().asString();
                if (m_creation)
                    trData["to"] = "";
                else
                    trData["to"] = m_to.getCContent().asString();
                trData["secretKey"] = m_secretKey.getCContent().asString();

                // Export Access List
                if (!databox.m_accessList.isEmpty() && databox.m_accessList.getCContent().list().size())
                    trData["accessList"] = databox.m_accessList.getCContent().asDataObject();

                out.push_back(
                    TransactionInGeneralSection(trData, dIND, gIND, vIND, databox.m_dataRawPreview, databox.m_dataLabel));
            }
        }
    }
    return out;
}
