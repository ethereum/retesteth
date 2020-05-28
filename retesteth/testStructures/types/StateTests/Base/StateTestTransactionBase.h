#pragma once
#include "../../../basetypes.h"
#include "TransactionInGeneralSection.h"
#include <retesteth/dataObject/DataObject.h>
#include <retesteth/dataObject/SPointer.h>

using namespace dataobject;
using namespace test::teststruct;
namespace test
{
namespace teststruct
{
// Base logic for State Test Transaction section
struct StateTestTransactionBase : GCP_SPointerBase
{
    DataObject const asDataObject() const;
    std::vector<TransactionInGeneralSection> buildTransactions() const;

protected:
    StateTestTransactionBase(){};
    std::vector<BYTES> m_data;
    std::vector<VALUE> m_gasLimit;
    std::vector<VALUE> m_value;
    spVALUE m_gasPrice;
    spVALUE m_nonce;
    spFH32 m_secretKey;
    spFH20 m_to;
    bool m_creation;
};


}  // namespace teststruct
}  // namespace test
