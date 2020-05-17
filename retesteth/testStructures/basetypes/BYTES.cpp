#include "BYTES.h"
#include <retesteth/EthChecks.h>
using namespace test::teststruct;

namespace test
{
namespace teststruct
{
BYTES::BYTES(DataObject const& _data) : m_data(_data)
{
    string const& k = _data.getKey();
    string const& v = _data.asString();
    if (v.size() < 2 || v[0] != '0' || v[1] != 'x')
        ETH_ERROR_MESSAGE("Key `" + k + "` is not BYTES `" + v + "`");
}

}  // namespace teststruct
}  // namespace test
