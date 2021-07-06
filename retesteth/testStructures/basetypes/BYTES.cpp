#include "BYTES.h"
#include <libdevcore/CommonIO.h>
#include <retesteth/EthChecks.h>
#include <retesteth/TestHelper.h>
#include <sstream>
using namespace test::teststruct;
using namespace dev;

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

string rlpToString(dev::RLP const& _rlp, size_t _minFieldSize, RLPTYPE _expected)
{
    std::ostringstream stream;
    stream << _rlp.toBytes();

    // Allow RLPs with bigint. Add marker in the json
    bool const prefix = (stream.str().size() > 64 + 2) && _expected == RLPTYPE::ALLOWBIGINT;

    return stream.str() == "0x" && _minFieldSize == 1 ? "0x00"
            : prefix ? "0x:bigint " + stream.str() : stream.str();
}

}  // namespace teststruct
}  // namespace test
