#include "BYTES.h"
#include <libdevcore/CommonIO.h>
#include <retesteth/EthChecks.h>
#include <retesteth/TestHelper.h>
#include <sstream>
using namespace test::teststruct;
using namespace dev;
using namespace std;

namespace
{
void toLowerHexStr(string& _input)
{
    std::transform(_input.begin(), _input.end(), _input.begin(), [](unsigned char c) {
        if (!isxdigit(c))
            ETH_ERROR_MESSAGE("BYTES string has char which is not hex: `" + string(1, c) + "`\n");
        return std::tolower(c);
    });
}
}  // namespace

namespace test
{
namespace teststruct
{
BYTES::BYTES(dev::RLP const& _rlp)
{
    std::ostringstream stream;
    stream << _rlp.toBytes();
    m_data = stream.str();
}

BYTES::BYTES(DataObject const& _data)
{
    string const& k = _data.getKey();
    string const& v = _data.asString();
    if (v.size() < 2 || v[0] != '0' || v[1] != 'x')
        ETH_ERROR_MESSAGE("Key `" + k + "` is not BYTES `" + v + "`");
    m_data = v.substr(2);
    toLowerHexStr(m_data);
    m_data = "0x" + m_data;
}

size_t BYTES::firstByte() const
{
    string const sFirstByte = asString().substr(2, 2);
    return std::strtol(sFirstByte.c_str(), NULL, 16);
}

}  // namespace teststruct
}  // namespace test
