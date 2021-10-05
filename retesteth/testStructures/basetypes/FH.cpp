#include "FH.h"
#include "../Common.h"
#include <libdevcore/CommonIO.h>
#include <libdevcore/RLP.h>
#include <retesteth/EthChecks.h>
#include <retesteth/TestHelper.h>
#include <sstream>
#include <mutex>

using namespace test;
using namespace test::teststruct;
using namespace dev;

std::mutex g_cacheAccessMutexFH;

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
FH::FH(dev::bigint const& _data, size_t _scale) : m_data(new VALUE(_data)), m_scale(_scale) {}

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
        m_data = spVALUE(new VALUE(dev::bigint(_data)));
    }
    else
    {
        // Validate hex
        // pos += 10;  // length of prefix
        try
        {
            if (validateHash(_data, m_scale))
                m_data = spVALUE(new VALUE(dev::bigint(_data.substr(pos + 10))));
            else
            {
                m_isCorrectHash = false;
                m_data = spVALUE(new VALUE(_data.substr(pos)));
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
    m_data = spVALUE(new VALUE(_rlp));
    m_scale = _scale;

    if (m_data->isBigInt())
    {
        size_t const gotScale = (m_data->asString().size() - 11) / 2;
        if (gotScale != _scale)
            m_isCorrectHash = false;
    }
    else
    {
        size_t const gotScale = (m_data->asString().size() - 1) / 2;
        if (gotScale != _scale)
            throw test::UpwardsException("Initializing FH" + test::fto_string(_scale) +
                                         " from RLP failed (check the string): `" + m_data->asString() +
                                         "` Reason: " + "Scale mismatch, got: " + test::fto_string(gotScale));
    }
}

string const& FH::asString() const
{
    if (m_data->isBigInt() && !m_isCorrectHash)
        return m_data->asString();

    if (m_dataStrZeroXCache.empty())
    {
        if (m_data->isBigInt())
            m_dataStrZeroXCache = m_data->asString().substr(10);
        else
            m_dataStrZeroXCache = m_data->asString();

        if (m_isCorrectHash)
            for (size_t size = m_dataStrZeroXCache.size() / 2; size <= m_scale; size++)
                m_dataStrZeroXCache.insert(2, "00");
    }

    return m_dataStrZeroXCache;
    /*
    if (m_bigint && _forRLP != ExportType::RLP)
        return m_bigintHash->asString();

    if (m_dataStrZeroXCache.empty())
    {
        std::lock_guard<std::mutex> lock(g_cacheAccessMutexFH);
        string& ret = m_dataStrZeroXCache;
        ret = m_data.str(1, std::ios_base::hex);
        test::strToLower(ret);
        if (ret.size() % 2 != 0)
            ret.insert(0, "0");

        if (!m_bigint)
        {
            for (size_t size = ret.size() / 2; size < m_scale; size++)
                ret.insert(0, "00");
        }

        m_dataStrZeroXCache.insert(0, "0x");
    }
    return m_dataStrZeroXCache; */
}

dev::bytes const& FH::serializeRLP() const
{
    if (m_rlpDataCache.empty())
        m_rlpDataCache = test::sfromHex(asString());

    if (m_data->isBigInt())
        return m_data->serializeRLP();
    return m_rlpDataCache;
}

}  // namespace teststruct
}  // namespace test
