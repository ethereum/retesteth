#include "VALUE.h"
#include <libdevcore/CommonIO.h>
#include <retesteth/EthChecks.h>
#include <retesteth/TestHelper.h>
#include <locale>
#include <mutex>
#include <sstream>

using namespace std;
using namespace dev;
using namespace test::teststruct;
std::mutex g_cacheAccessMutexValue;

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
    auto const str = stream.str();
    for (size_t i = 2; i < str.size() - 2; i++)
    {
        if (str.at(i) == '0')
            m_prefixedZeros++;
        else
            break;
    }

    m_bigint = (str.size() > 64 + 2) || m_prefixedZeros >= 2;
    m_data = dev::bigint(str);
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
        throw test::UpwardsException("VALUE element must be at least 0x prefix" + suffix);

    if (_s[0 + pos] == '0' && _s[1 + pos] == 'x')
    {
        if (pos == 0)
        {
            size_t const fixedsize = _s.size() - pos;
            if (fixedsize - pos == 2)
                throw test::UpwardsException("VALUE set as empty byte string: `" + _s + "`" + suffix);

            // don't allow 0x001, but allow 0x0, 0x00
            if ((_s[2 + pos] == '0' && fixedsize % 2 == 1 && fixedsize != 3) ||
                (_s[2 + pos] == '0' && _s[3 + pos] == '0' && fixedsize % 2 == 0 && fixedsize > 4))
                throw test::UpwardsException("VALUE has leading 0 `" + _s + "`" + suffix);

            else if (fixedsize > 64 + 2)
                throw test::UpwardsException("VALUE  >u256 `" + _s + "`" + suffix);
        }
        else
        {
            m_prefixedZeros = 0;
            const string ret = _s.substr(pos);
            if (ret.size() == 2)
                m_bigintEmpty = true;

            for (size_t i = 2; i < ret.size() - 2; i++)
            {
                if (ret.at(i) == '0')
                    m_prefixedZeros++;
                else
                    break;
            }

            // Indicates bigint
            return ret;
        }
    }
    else
        throw test::UpwardsException("VALUE is not prefixed hex `" + _s + "`" + suffix);

    return string();
}

string VALUE::asDecString() const
{
    return m_data.str(0, std::ios_base::dec);
}

string const& VALUE::asString() const
{
    calculateCache();
    return m_bigint ? m_dataStrBigIntCache : m_dataStrZeroXCache;
}

dev::bytes const& VALUE::serializeRLP() const
{
    calculateCache();
    if (m_bigint)
        return m_bytesBigIntData;
    return m_bytesData;
}

void VALUE::calculateCache() const
{
    std::lock_guard<std::mutex> lock(g_cacheAccessMutexValue);
    if (m_dirty)
    {
        m_dirty = false;
        string& ret = m_dataStrZeroXCache;

        if (m_bigintEmpty)
            ret = "";
        else
        {
            ret = m_data.str(0, std::ios_base::hex);
            if (ret.size() % 2 != 0)
                ret.insert(0, "0");
            test::strToLower(ret);
        }

        if (m_bigint)
        {
            size_t existingZero = 0;
            string prefixedZero;
            if (m_dataStrZeroXCache.size() > 2 && m_dataStrZeroXCache.at(0) == '0' && m_prefixedZeros > 0)
                existingZero = 1;
            for (size_t i = 0; i < m_prefixedZeros - existingZero; i += 1)
                prefixedZero.insert(0, "0");

            m_dataStrBigIntCache = m_dataStrZeroXCache;
            m_dataStrZeroXCache.insert(0, "0x");
            m_dataStrBigIntCache.insert(0, prefixedZero);
            m_dataStrBigIntCache.insert(0, "0x:bigint 0x");
            m_bytesBigIntData = test::sfromHex(prefixedZero) + test::sfromHex(m_dataStrZeroXCache);
        }
        else
        {
            m_dataStrZeroXCache.insert(0, "0x");
            m_bytesData = (m_data == 0) ? test::sfromHex("") : test::sfromHex(m_dataStrZeroXCache);
        }
    }
}

}  // namespace teststruct
}  // namespace test
