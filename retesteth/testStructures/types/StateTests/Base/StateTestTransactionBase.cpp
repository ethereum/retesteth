#include "StateTestTransactionBase.h"
#include <retesteth/ExitHandler.h>

using namespace dataobject;
using namespace test::teststruct;

const DataObject StateTestTransactionBase::asDataObject() const
{
    // Serialize data back to JSON
    DataObject out;
    for (BYTES const& el : m_data)
        out["data"].addArrayObject(el.asString());
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
    for (size_t dIND = 0; dIND < m_data.size(); dIND++)
    {
        for (size_t gIND = 0; gIND < m_gasLimit.size(); gIND++)
        {
            for (size_t vIND = 0; vIND < m_value.size(); vIND++)
            {
                if (ExitHandler::receivedExitSignal())
                    return out;
                DataObject trData;
                trData["data"] = m_data.at(dIND).asString();
                trData["gasLimit"] = m_gasLimit.at(gIND).asString();
                trData["value"] = m_value.at(vIND).asString();

                trData["gasPrice"] = m_gasPrice.getCContent().asString();
                trData["nonce"] = m_nonce.getCContent().asString();
                if (m_creation)
                    trData["to"] = "";
                else
                    trData["to"] = m_to.getCContent().asString();
                trData["secretKey"] = m_secretKey.getCContent().asString();

                string const dataLabel = m_dataLabel.size() > dIND ? m_dataLabel.at(dIND) : string();
                string const dataRawPreview = m_dataRawPreview.size() > dIND ? m_dataRawPreview.at(dIND) : string();
                out.push_back(TransactionInGeneralSection(trData, dIND, gIND, vIND, dataRawPreview, dataLabel));
            }
        }
    }
    return out;
}
