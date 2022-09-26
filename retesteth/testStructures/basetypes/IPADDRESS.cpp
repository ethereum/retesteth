#include "IPADDRESS.h"
#include <retesteth/EthChecks.h>
#include <boost/asio.hpp>
using namespace std;
using namespace test::teststruct;

namespace test
{
namespace teststruct
{
IPADDRESS::IPADDRESS(DataObject const& _ip) : m_data(_ip.asString())
{
    size_t pos = _ip.asString().find_last_of(':');
    string address = _ip.asString().substr(0, pos);
    int port = atoi(_ip.asString().substr(pos + 1).c_str());
    boost::system::error_code ec;
    boost::asio::ip::address::from_string(address, ec);
    if (port <= 1024 || port > 49151 || ec)
        ETH_ERROR_MESSAGE("Key `" + _ip.getKey() + "` is not IPADDRESS `" + _ip.asString() + "`");
}

}  // namespace teststruct
}  // namespace test
