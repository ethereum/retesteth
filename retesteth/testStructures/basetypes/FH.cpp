#include "../Common.h"
#include <retesteth/EthChecks.h>
#include <retesteth/TestHelper.h>
using namespace test;
using namespace test::teststruct;
using namespace dev;
using namespace std;

std::mutex g_cacheAccessMutexFH;

namespace
{
bool validateHash(std::string const& _hash, size_t _size)
{
    // validate 0x...... _size bytes hash
    if (_hash.size() != _size * 2 + 2) // || stringIntegerType(_hash) != DigitsType::HexPrefixed)
        return false;
    return true;
}
}  // namespace

namespace test
{
namespace teststruct
{

void FH::_initialize(string const& _data, string const& _key)
{
    string const scale = to_string(m_scale);
    size_t const pos = _data.find("0x:bigint ");
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
        m_data = BYTES(_data);
    }
    else
    {
        // Validate hex
        // pos += 10;  // length of prefix
        try
        {
            if (validateHash(_data, m_scale))
                m_data = BYTES(_data.substr(pos + 10));
            else
            {
                m_isCorrectHash = false;
                m_data = BYTES(_data.substr(pos + 10));
            }
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
    m_data = BYTES(_rlp);
    m_scale = _scale;

    size_t const gotScale = (m_data.asString().size() - 2) / 2;

    if (gotScale != _scale)
        m_isCorrectHash = false;
}

string const& FH::asString() const
{
    std::lock_guard<std::mutex> lock(g_cacheAccessMutexFH);
    if (m_dataStrZeroXCache.empty())
    {
        if (m_isCorrectHash)
            m_dataStrZeroXCache = m_data.asString();
        else
        {
            m_dataStrZeroXCache = m_data.asString();
            m_dataStrZeroXCache.insert(0, "0x:bigint ");
        }
    }
    return m_dataStrZeroXCache;
}

dev::bytes const& FH::serializeRLP() const
{
    if (m_rlpDataCache.empty())
        m_rlpDataCache = test::sfromHex(m_data.asString());
    return m_rlpDataCache;
}

}  // namespace teststruct
}  // namespace test
