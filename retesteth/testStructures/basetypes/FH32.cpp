#include "FH32.h"
#include <retesteth/EthChecks.h>
#include <retesteth/ethObjects/object.h>
using namespace test::teststruct;


namespace test
{
namespace teststruct
{
FH32::FH32(DataObject const& _data) : m_data(_data)
{
    if (!object::validateHash(_data.asString(), 32))
        ETH_ERROR_MESSAGE("Key `" + _data.getKey() + "` is not hash32 `" + _data.asString() + "`");
}

}  // namespace teststruct
}  // namespace test
