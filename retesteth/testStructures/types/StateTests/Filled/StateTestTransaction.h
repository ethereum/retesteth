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
struct StateTestTransaction : GCP_SPointerBase
{
    StateTestTransaction(DataObject const&);
    DataObject asDataObject() const;
    std::vector<TransactionInGeneralSection> buildTransactions() const;

private:
    std::vector<BYTES> m_data;
    std::vector<VALUE> m_gasLimit;
    std::vector<VALUE> m_value;
    spVALUE m_gasPrice;
    spVALUE m_nonce;
    spFH32 m_secretKey;
    spFH20 m_to;
};

}  // namespace teststruct
}  // namespace test
