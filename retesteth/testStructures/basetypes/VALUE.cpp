#include "VALUE.h"
#include <retesteth/EthChecks.h>
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

VALUE::VALUE(dev::u256 _data) : m_data(_data) {}

VALUE::VALUE(int _data)
{
    m_data = dev::u256(_data);
}

VALUE::VALUE(DataObject const& _data)
{
    if (_data.type() == DataType::Integer)
        m_data = _data.asInt();
    else
    {
        verifyHexString(_data.asString(), _data.getKey());
        m_data = dev::u256(_data.asString());
    }
}

void VALUE::verifyHexString(std::string const& _s, std::string const& _k) const
{
    string const suffix = _k.empty() ? _k : " (key: " + _k + " )";
    if (_s[0] == '0' && _s[1] == 'x')
    {
        if (_s.size() == 2)
            throw test::UpwardsException("VALUE set as empty byte string: `" + _s + "`" + suffix);
        // don't allow 0x001, but allow 0x0, 0x00
        if ((_s[2] == '0' && _s.size() % 2 == 1 && _s.size() != 3) ||
            (_s[2] == '0' && _s[3] == '0' && _s.size() % 2 == 0 && _s.size() > 4))
            throw test::UpwardsException("VALUE has leading 0 `" + _s + "`" + suffix);

        if (_s.size() > 64 + 2)
            throw test::UpwardsException("VALUE  >u256 `" + _s + "`" + suffix);
    }
    else
        throw test::UpwardsException("VALUE is not prefixed hex `" + _s + "`" + suffix);
}

string VALUE::asDecString() const
{
    return m_data.str(0, std::ios_base::dec);
}

}  // namespace teststruct
}  // namespace test
