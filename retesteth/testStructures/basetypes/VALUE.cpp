#include "VALUE.h"
#include <retesteth/EthChecks.h>
#include <retesteth/TestHelper.h>
#include <locale>
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

VALUE::VALUE(dev::bigint const& _data) : m_data(_data) {}

VALUE::VALUE(int _data)
{
    m_data = dev::bigint(_data);
}

VALUE::VALUE(DataObject const& _data)
{
    if (_data.type() == DataType::Integer)
        m_data = _data.asInt();
    else
    {
        string const correct = verifyHexString(_data.asString(), _data.getKey());
        if (correct.size())
        {
            m_bigint = true;
            m_data = dev::bigint(correct);
        }
        else
            m_data = dev::bigint(_data.asString());
    }
}

string VALUE::verifyHexString(std::string const& _s, std::string const& _k) const
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

        size_t pos = _s.find(":bigint");
        if (pos != string::npos)
            return _s.substr(pos + 8);
        else if (_s.size() > 64 + 2)
            throw test::UpwardsException("VALUE  >u256 `" + _s + "`" + suffix);
    }
    else
        throw test::UpwardsException("VALUE is not prefixed hex `" + _s + "`" + suffix);

    return string();
}

string VALUE::asDecString() const
{
    return m_data.str(0, std::ios_base::dec);
}

string VALUE::asString(size_t _roundBytes, bool _noMarker) const
{
    string ret = m_data.str(_roundBytes, std::ios_base::hex);
    if (ret.size() % 2 != 0)
        ret = "0" + ret;
    test::strToLower(ret);
    return m_bigint && !_noMarker ? "0x:bigint 0x" + ret : "0x" + ret;
    // return dev::toCompactHexPrefixed(m_data, _roundBytes);
}

}  // namespace teststruct
}  // namespace test
