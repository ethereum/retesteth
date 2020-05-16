#pragma once
#include "../../basetypes/BYTES.h"
#include "../../basetypes/FH20.h"
#include "../../basetypes/FH32.h"
#include "../../basetypes/VALUE.h"
#include <retesteth/dataObject/DataObject.h>
#include <retesteth/dataObject/SPointer.h>
using namespace dataobject;
using namespace test::teststruct;

namespace test
{
namespace teststruct
{
struct StateTestFillerTransaction : GCP_SPointerBase
{
    StateTestFillerTransaction(DataObject const&);

private:
    std::vector<BYTES> m_data;
    std::vector<VALUE> m_gasLimit;
    std::vector<VALUE> m_value;
    VALUE m_gasPrice;
    VALUE m_nonce;
    FH32 m_secretKey;
    FH20 m_to;
};


}  // namespace teststruct
}  // namespace test
