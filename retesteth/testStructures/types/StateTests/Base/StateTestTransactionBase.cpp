#include "StateTestTransactionBase.h"
#include <retesteth/EthChecks.h>
#include <retesteth/ExitHandler.h>

using namespace std;
using namespace dataobject;
using namespace test::teststruct;

spDataObject StateTestTransactionBase::asDataObject() const
{
    return m_rawData;
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
                if (ExitHandler::receivedExitSignal())
                    return out;

                Databox const& databox = m_databox.at(dIND);

                spDataObject trData;
                (*trData)["data"] = databox.m_data.asString();
                (*trData)["gasLimit"] = m_gasLimit.at(gIND).asString();
                (*trData)["value"] = m_value.at(vIND).asString();
                (*trData)["nonce"] = m_nonce->asString();
                if (m_creation)
                    (*trData)["to"] = "";
                else
                    (*trData)["to"] = m_to->asString();

                (*trData)["secretKey"] = m_secretKey->asString();
                if (!m_publicKey.isEmpty())
                    (*trData)["sender"] = m_publicKey->asString();

                // EIP 1559
                if (!m_maxPriorityFeePerGas.isEmpty())
                {
                    // Type 0x02 transaction fields
                    (*trData)["maxPriorityFeePerGas"] = m_maxPriorityFeePerGas->asString();
                    (*trData)["maxFeePerGas"] = m_maxFeePerGas->asString();

                    if (databox.m_accessList.isEmpty())
                        ETH_FAIL_MESSAGE("BaseFeeTransaction must have access list!");
                }
                else
                {
                    (*trData)["gasPrice"] = m_gasPrice->asString();
                }

                // Export Access List
                if (!databox.m_accessList.isEmpty())
                    (*trData).atKeyPointer("accessList") = databox.m_accessList->asDataObject();

                out.push_back(
                    TransactionInGeneralSection(dataobject::move(trData), dIND, gIND, vIND, databox.m_dataRawPreview, databox.m_dataLabel));
            }
        }
    }
    return out;
}
