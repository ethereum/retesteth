#include "VALUE.h"
#include <retesteth/EthChecks.h>
#include <retesteth/ethObjects/object.h>
#include <retesteth/TestHelper.h>
using namespace test::teststruct;

namespace test
{
namespace teststruct
{
// if int  make u256(int)
// if sring
//     if string is dec  make string hex
//     if string is hex  check hexvalue 0x00000 leading zeros etc
// check limit

VALUE::VALUE(dev::u256 _data, dev::u256 const& _limit) : m_data(_data)
{
    checkLimit(_limit);
}

VALUE::VALUE(int _data, dev::u256 const& _limit)
{
    m_data = dev::u256(_data);
    checkLimit(_limit);
}

VALUE::VALUE(DataObject const& _data, dev::u256 const& _limit)
{
    if (_data.type() == DataType::Integer)
        m_data = _data.asInt();
    else
    {
        verifyHexString(_data.asString(), _data.getKey());
        m_data = dev::u256(_data.asString());
    }
    checkLimit(_limit);
}

void VALUE::verifyHexString(std::string const& _s, std::string const& _k) const
{
    string const suffix = _k.empty() ? _k : " (key: " + _k + " )";
    if (_s[0] == '0' && _s[1] == 'x')
    {
        if (_s.size() == 2)
            ETH_ERROR_MESSAGE("VALUE set as empty byte string: `" + _s + "`" + suffix);
        // don't allow 0x001, but allow 0x0, 0x00
        if ((_s[2] == '0' && _s.size() % 2 == 1 && _s.size() != 3) ||
            (_s[2] == '0' && _s[3] == '0' && _s.size() % 2 == 0 && _s.size() > 4))
            ETH_ERROR_MESSAGE("VALUE has leading 0 `" + _s + "`" + suffix);

        if (_s.size() > 64 + 2)
            ETH_ERROR_MESSAGE("VALUE  >u256 `" + _s + "`" + suffix);
    }
    else
        ETH_ERROR_MESSAGE("VALUE is not prefixed hex `" + _s + "`" + suffix);
}

void VALUE::checkLimit(dev::u256 const& _limit) const
{
    if (m_data > _limit)
        ETH_ERROR_MESSAGE(
            "VALUE `" + dev::toCompactHexPrefixed(m_data, 1) + "`  >limit `" + dev::toCompactHexPrefixed(_limit, 1) + "`");
}

string VALUE::asDecString() const
{
    return m_data.str(0, std::ios_base::dec);
}

}  // namespace teststruct
}  // namespace test
