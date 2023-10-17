#include "StateTestTransactionBase.h"
#include <retesteth/EthChecks.h>
#include <retesteth/ExitHandler.h>
#include <retesteth/Constants.h>

using namespace std;
using namespace dataobject;
using namespace test::teststruct;
using namespace test::teststruct::constnames;

spDataObject StateTestTransactionBase::asDataObject() const
{
    // Saves serialization time because processed in the constructor
    // TODO: double check this
    return m_rawData;
}

/// Construct individual transactions from gstate test transaction
std::vector<TransactionInGeneralSection> StateTestTransactionBase::buildTransactions() const
{
    // Construct vector of all transactions that are described int data
    std::vector<TransactionInGeneralSection> out;
    out.reserve(m_databox.size() * m_gasLimit.size() * m_value.size());
    for (size_t dIND = 0; dIND < m_databox.size(); dIND++)
    {
        for (size_t gIND = 0; gIND < m_gasLimit.size(); gIND++)
        {
            for (size_t vIND = 0; vIND < m_value.size(); vIND++)
            {
                CHECKEXITR(out)
                Databox const& databox = m_databox.at(dIND);

                spDataObject trData;
                (*trData)[c_data] = databox.m_data.asString();
                (*trData)[c_gasLimit] = m_gasLimit.at(gIND).asString();
                (*trData)[c_value] = m_value.at(vIND).asString();
                (*trData)[c_nonce] = m_nonce->asString();
                if (m_creation)
                    (*trData)[c_to] = "";
                else
                    (*trData)[c_to] = m_to->asString();

                (*trData)[c_secretKey] = m_secretKey->asString();
                if (!m_publicKey.isEmpty())
                    (*trData)[c_sender] = m_publicKey->asString();

                if (!m_maxPriorityFeePerGas.isEmpty())
                {
                    // Type 0x02 transaction fields
                    (*trData)[c_maxPriorityFeePerGas] = m_maxPriorityFeePerGas->asString();
                    (*trData)[c_maxFeePerGas] = m_maxFeePerGas->asString();

                    if (databox.m_accessList.isEmpty())
                        ETH_FAIL_MESSAGE("BaseFeeTransaction must have access list!");

                    // Type 0x03 transaction fields
                    if (!m_maxFeePerBlobGas.isEmpty())
                    {
                        (*trData)[c_maxFeePerBlobGas] = m_maxFeePerBlobGas->asString();
                        (*trData).atKeyPointer(c_blobVersionedHashes) = sDataObject(DataType::Array);
                        for (auto const& el : m_blobVersionedHashes)
                            (*trData)[c_blobVersionedHashes].addArrayObject(sDataObject(el.asString()));
                    }

                }
                else
                {
                    (*trData)[c_gasPrice] = m_gasPrice->asString();
                }

                // Export Access List
                if (!databox.m_accessList.isEmpty())
                    (*trData).atKeyPointer(c_accessList) = databox.m_accessList->asDataObject();

                out.emplace_back(TransactionInGeneralSection(dataobject::move(trData), dIND, gIND, vIND, databox.m_dataRawPreview, databox.m_dataLabel));
            }
        }
    }
    return out;
}
