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
    auto const& str = stream.str();
    m_prefixedZeroBytes = _countPrefixedBytes(str);
    m_bigint = (str.size() > 64 + 2) || m_prefixedZeroBytes >= 1;
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
    string const& bigIntPrefix = C_BIGINT_PREFIX;
    string const suffix = _k.empty() ? _k : " (key: " + _k + " )";

    size_t bigIntOffset = 0;
    if (auto foundPos = _s.find(bigIntPrefix); foundPos != string::npos)
        bigIntOffset = foundPos + bigIntPrefix.size();

    if (_s.size() < 2 || _s.size() - bigIntOffset < 2)
        throw test::UpwardsException("VALUE element must be at least 0x prefix" + suffix);

    if (_s.at(0 + bigIntOffset) == '0' && _s.at(1 + bigIntOffset) == 'x')
    {
        if (bigIntOffset == 0)
        {
            if (_s.size() == 2)
                throw test::UpwardsException("VALUE set as empty byte string: `" + _s + "`" + suffix);

            // Allow 0x0 and 0x00 but don't allow 0x012, 0x000
            const bool hasFirstZero = _s.size() > 2 && _s.at(2) == '0';
            const bool hasSecondZero = _s.size() > 3 && _s.at(3) == '0';

            if (_s.size() % 2 == 0 && _s.size() > 4 && hasFirstZero && hasSecondZero)
                throw test::UpwardsException("VALUE has leading 0 `" + _s + "`" + suffix);

            if (_s.size() % 2 == 1 && _s.size() > 3 && hasFirstZero)
                throw test::UpwardsException("VALUE has leading 0 `" + _s + "`" + suffix);

            if (_s.size() > 64 + 2)
                throw test::UpwardsException("VALUE  >u256 `" + _s + "`" + suffix);
        }
        else
        {
            const string ret = _s.substr(bigIntOffset);
            if (ret.size() == 2)
                m_bigintEmpty = true;

            m_prefixedZeroBytes = _countPrefixedBytes(ret);
            return ret; // Indicates bigint
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
    return m_dataStr;
}

dev::bytes const& VALUE::serializeRLP() const
{
    calculateCache();
    return m_bytesData;
}

void VALUE::calculateCache() const
{
    std::lock_guard<std::mutex> lock(g_cacheAccessMutexValue);
    if (m_dirty)
    {
        m_dirty = false;

        m_dataStr = m_data.str(0, std::ios_base::hex);
        if (m_dataStr.size() % 2 != 0)
            m_dataStr.insert(0, "0");
        test::strToLower(m_dataStr);

        if (!m_bigint)
        {
            m_dataStr.insert(0, "0x");
            m_bytesData = (m_data == 0) ? test::sfromHex("") : test::sfromHex(m_dataStr);
        }
        else
        {
            if (m_prefixedZeroBytes > 0)
            {
                string const padding(m_prefixedZeroBytes * 2, '0');
                m_dataStr.insert(0, padding);
            }
            m_dataStr.insert(0, "0x");
            m_bytesData = m_bigintEmpty ? test::sfromHex("") : test::sfromHex(m_dataStr);
            m_dataStr.insert(0, C_BIGINT_PREFIX);
        }
    }
}

size_t VALUE::_countPrefixedBytes(std::string const& _str) const
{
    if (_str.size() < 5) // 0x00 does not count
        return 0;

    size_t zeros = 0;
    for (string::const_iterator it = _str.begin() + 2; it != _str.end() - 2; it++)
    {
        if ((*it) == '0')
            zeros++;
        else
        {
            // 0x000122 => "0x000122" (1 byte, 2 zeros)
            // 0x0001122 => "0x00001122 (2 byte, 3 zeros)
            // 0x00001122 => "0x00001122 (2 byte, 4 zeros)
            if (zeros % 2 == 1 && zeros > 0)
                zeros--;
            break;
        }
    }
    return std::ceil((double)zeros / 2);
}


}  // namespace teststruct
