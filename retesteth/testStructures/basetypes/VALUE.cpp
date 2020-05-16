#include "VALUE.h"
#include <retesteth/EthChecks.h>
using namespace test::teststruct;

namespace test
{
namespace teststruct
{
VALUE::VALUE(DataObject const& _data, dev::u256 _limit) : m_data(_data)
{
    string const& k = _data.getKey();
    string const& v = _data.asString();
    if (v[0] == '0' && v[1] == 'x')
    {
        if (v.size() == 2)
            ETH_ERROR_MESSAGE(
                "Key `" + k + "` is value, but set as empty byte string: `" + v + "`");
        // don't allow 0x001, but allow 0x0, 0x00
        if ((v[2] == '0' && v.size() % 2 == 1 && v.size() != 3) ||
            (v[2] == '0' && v[3] == '0' && v.size() % 2 == 0 && v.size() > 4))
            ETH_ERROR_MESSAGE("Key `" + k + "` has leading 0 `" + v + "`");
    }
    if (v.size() > 64 + 2)
        ETH_ERROR_MESSAGE("Key `" + k + "` >u256 `" + v + "`");
    if (dev::u256(v) > _limit)
        ETH_ERROR_MESSAGE(
            "Key `" + k + "` > limit `" + dev::toString(_limit) + "`, key = `" + v + "`");
}

}  // namespace teststruct
}  // namespace test
