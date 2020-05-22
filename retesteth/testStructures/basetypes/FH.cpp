#include "FH.h"
#include <retesteth/EthChecks.h>
#include <retesteth/TestHelper.h>
#include <retesteth/ethObjects/object.h>
using namespace test::teststruct;

namespace test
{
namespace teststruct
{
FH::FH(string const& _data, size_t _scale) : m_data(_data), m_scale(_scale)
{
    string const scale = to_string(m_scale);
    if (!object::validateHash(m_data, m_scale))
        ETH_ERROR_MESSAGE("Initializing FH" + scale + " from string that is not hash" + scale + " `" + m_data + "`");
    strToLower(m_data);
}

FH::FH(DataObject const& _data, size_t _scale) : m_scale(_scale)
{
    string const scale = to_string(_scale);
    if (!object::validateHash(_data.asString(), m_scale))
        ETH_ERROR_MESSAGE("Key `" + _data.getKey() + "` is not hash" + scale + " `" + _data.asString() + "`");
    m_data = _data.asString();
    strToLower(m_data);
}

}  // namespace teststruct
}  // namespace test
