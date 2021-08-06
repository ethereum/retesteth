#include "VALUE.h"
#include <libdevcore/CommonIO.h>
#include <retesteth/EthChecks.h>
#include <retesteth/TestHelper.h>
#include <locale>
#include <sstream>

using namespace test::teststruct;
using namespace dev;

namespace test
{
namespace teststruct
{
// if int  make u256(int)
// if sring
//     if string is dec  make string hex
//     if string is hex  check hexvalue 0x00000 leading zeros etc
// check limit

VALUE::VALUE(dev::RLP const& _rlp)
{
    std::ostringstream stream;
    stream << _rlp.toBytes();
    m_bigint = (stream.str().size() > 64 + 2);
    m_data = dev::bigint(stream.str());
}

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
        string const withoutKeyWord = verifyHexString(_data.asString(), _data.getKey());
        if (withoutKeyWord.size())
        {
            m_bigint = true;
            m_data = dev::bigint(withoutKeyWord);
        }
        else
            m_data = dev::bigint(_data.asString());
    }
}

string VALUE::verifyHexString(std::string const& _s, std::string const& _k) const
{
    string const suffix = _k.empty() ? _k : " (key: " + _k + " )";

    static string const prefix = "0x:bigint ";
    size_t pos = _s.find(prefix);
    if (pos != string::npos)
        pos = pos + prefix.size();
    else
        pos = 0;

    if (_s.size() - pos < 2)
        throw test::UpwardsException("VALUE element must be at leas 0x prefix" + suffix);

    if (_s[0 + pos] == '0' && _s[1 + pos] == 'x')
    {
        size_t const fixedsize = _s.size() - pos;
        if (fixedsize - pos == 2)
            throw test::UpwardsException("VALUE set as empty byte string: `" + _s + "`" + suffix);

        // don't allow 0x001, but allow 0x0, 0x00
        if ((_s[2 + pos] == '0' && fixedsize % 2 == 1 && fixedsize != 3) ||
            (_s[2 + pos] == '0' && _s[3 + pos] == '0' && fixedsize % 2 == 0 && fixedsize > 4))
            throw test::UpwardsException("VALUE has leading 0 `" + _s + "`" + suffix);

        else if (fixedsize > 64 + 2 && pos == 0)
            throw test::UpwardsException("VALUE  >u256 `" + _s + "`" + suffix);

        if (pos > 0)
            return _s.substr(pos);
    }
    else
        throw test::UpwardsException("VALUE is not prefixed hex `" + _s + "`" + suffix);

    return string();
}

string VALUE::asDecString() const
{
    return m_data.str(0, std::ios_base::dec);
}

string VALUE::asString(size_t _roundBytes) const
{
    string ret = m_data.str(_roundBytes, std::ios_base::hex);
    if (ret.size() % 2 != 0)
        ret = "0" + ret;
    test::strToLower(ret);
    return m_bigint ? "0x:bigint 0x" + ret : "0x" + ret;
}

}  // namespace teststruct
}  // namespace test
