#include "FH20.h"
#include <retesteth/EthChecks.h>
#include <retesteth/ethObjects/object.h>
using namespace test::teststruct;

namespace test
{
namespace teststruct
{
FH20::FH20(DataObject const& _data) : m_data(_data)
{
    if (!object::validateHash(_data.asString(), 20))
        ETH_ERROR_MESSAGE("Key `" + _data.getKey() + "` is not hash20 `" + _data.asString() + "`");
    m_data.performModifier(mod_valuesToLowerCase);
}

}  // namespace teststruct
}  // namespace test
