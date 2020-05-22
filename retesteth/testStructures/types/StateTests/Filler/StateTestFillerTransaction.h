#pragma once
#include "../../../basetypes.h"
#include "../../../types/ethereum.h"
#include "../TransactionInGeneralSection.h"
#include <retesteth/dataObject/DataObject.h>
#include <retesteth/dataObject/SPointer.h>
using namespace dataobject;
using namespace test::teststruct;

namespace test
{
namespace teststruct
{

// Sructure to store JSON data from test
struct StateTestFillerTransaction : GCP_SPointerBase
{
    StateTestFillerTransaction(DataObject const&);
    DataObject asDataObject() const;
    std::vector<TransactionInGeneralSection> buildTransactions() const;

private:
    std::vector<BYTES> m_data;
    std::vector<VALUE> m_gasLimit;
    std::vector<VALUE> m_value;
    GCP_SPointer<VALUE> m_gasPrice;
    GCP_SPointer<VALUE> m_nonce;
    GCP_SPointer<FH32> m_secretKey;
    GCP_SPointer<FH20> m_to;
};

}  // namespace teststruct
}  // namespace test
