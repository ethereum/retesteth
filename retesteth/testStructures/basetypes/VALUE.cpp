#include "VALUE.h"
#include <libdevcore/CommonIO.h>
#include <retesteth/EthChecks.h>
#include <retesteth/TestHelper.h>
#include <retesteth/Constants.h>

using namespace std;
using namespace dev;
using namespace test::teststruct;
std::mutex g_cacheAccessMutexValue;

namespace test::teststruct
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

VALUE::VALUE(string const& _data)
{
    _fromString(_data);
}

VALUE::VALUE(DataObject const& _data)
{
    if (_data.type() == DataType::Integer)
        m_data = _data.asInt();
    else
        _fromString(_data.asString(), _data.getKey());
}

void VALUE::_fromString(std::string const& _data, std::string const& _hintkey)
{
    string const withoutKeyWord = verifyHexString(_data, _hintkey);
    if (withoutKeyWord.size())
    {
        m_bigint = true;
        m_data = dev::bigint(withoutKeyWord);
    }
    else
        m_data = dev::bigint(_data);
}

string VALUE::verifyHexString(std::string const& _s, std::string const& _k) const
{
    string const& prefix = C_BIGINT_PREFIX;
    string const suffix = _k.empty() ? _k : " (key: " + _k + " )";

    size_t pos = 0;
    if (auto foundPos = _s.find(prefix); foundPos != string::npos)
        pos = foundPos + prefix.size();

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
            unsigned short addZeroesNumber = m_prefixedZeros;
            if (m_dataStrZeroXCache.size() > 2 && m_dataStrZeroXCache.at(0) == '0' && addZeroesNumber > 0)
                addZeroesNumber -= 1;
            string prefixedZero(addZeroesNumber, '0');

            m_dataStrBigIntCache = m_dataStrZeroXCache;
            m_dataStrZeroXCache.insert(0, "0x");
            m_dataStrBigIntCache.insert(0, prefixedZero);
            m_dataStrBigIntCache.insert(0, C_BIGINT_PREFIX);
            m_dataStrBigIntCache.insert(0, "0x");
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
