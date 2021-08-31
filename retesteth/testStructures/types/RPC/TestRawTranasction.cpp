#include "TestRawTranasction.h"
#include <retesteth/EthChecks.h>
#include <retesteth/testStructures/Common.h>

using namespace dataobject;

namespace test
{
namespace teststruct
{
TestRawTransaction::TestRawTransaction(DataObject const& _data)
{
    requireJsonFields(_data, "TestRawTransaction " + _data.getKey(),
        {{"sender", {{DataType::String}, jsonField::Required}}, {"error", {{DataType::String}, jsonField::Required}}});

    m_error = _data.atKey("error").asString();
    m_sender = spFH20(new FH20(_data.atKey("sender")));
}

}  // namespace teststruct
}  // namespace test
