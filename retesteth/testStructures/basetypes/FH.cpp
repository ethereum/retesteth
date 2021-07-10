#include "FH.h"
#include "../Common.h"
#include <libdevcore/CommonIO.h>
#include <libdevcore/RLP.h>
#include <retesteth/EthChecks.h>
#include <retesteth/TestHelper.h>
#include <sstream>
using namespace test;
using namespace test::teststruct;
using namespace dev;

namespace
{
bool validateHash(std::string const& _hash, size_t _size)
{
    // validate 0x...... _size bytes hash
    if (_hash.size() != _size * 2 + 2 || stringIntegerType(_hash) != DigitsType::HexPrefixed)
        return false;
    return true;
}
}  // namespace

namespace test
{
namespace teststruct
{
// Fast constructor without any checks
FH::FH(dev::bigint const& _data, size_t _scale) : m_data(_data), m_scale(_scale) {}

void FH::_initialize(string const& _data, string const& _key)
{
    m_bigint = false;
    string const scale = to_string(m_scale);
    size_t pos = _data.find("0x:bigint ");
    if (pos == string::npos)
    {
        if (!validateHash(_data, m_scale))
        {
            if (_key.empty())
                throw test::UpwardsException(
                    "Initializing FH" + scale + " from string that is not hash" + scale + " `" + _data + "`");
            else
                throw test::UpwardsException("Key `" + _key + "` is not hash" + scale + " `" + _data + "`");
        }
        m_data = dev::bigint(_data);
    }
    else
    {
        // Validate hex
        pos += 10;  // length of prefix
        try
        {
            VALUE v(_data.substr(pos));
            m_data = v.asBigInt();
            m_bigint = true;
        }
        catch (std::exception const& _ex)
        {
            string const key = _key.empty() ? string() : "(Key: `" + _key + "`)";
            throw test::UpwardsException("Initializing FH" + scale + " " + key + " from bigint failed (check the string): `" +
                                         _data + "` Reason: " + _ex.what());
        }
    }
}

FH::FH(string const& _data, size_t _scale) : m_scale(_scale)
{
    _initialize(_data);
}

FH::FH(DataObject const& _data, size_t _scale) : m_scale(_scale)
{
    _initialize(_data.asString(), _data.getKey());
}

FH::FH(dev::RLP const& _rlp, size_t _scale)
{
    std::ostringstream stream;
    stream << _rlp.toBytes();
    auto const str = stream.str();
    m_bigint = (str.size() != _scale * 2 + 2);
    m_data = dev::bigint(str);
    m_scale = _scale;
}

string FH::asString(bool _forRLP) const
{
    string ret = m_data.str(1, std::ios_base::hex);
    test::strToLower(ret);
    if (ret.size() % 2 != 0)
        ret = "0" + ret;

    if (!m_bigint)
    {
        for (size_t size = ret.size() / 2; size < m_scale; size++)
            ret = "00" + ret;
    }

    return m_bigint && !_forRLP ? "0x:bigint 0x" + ret : "0x" + ret;
}

}  // namespace teststruct
}  // namespace test
