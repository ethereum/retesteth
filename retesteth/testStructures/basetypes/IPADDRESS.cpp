#include "IPADDRESS.h"
#include <retesteth/EthChecks.h>
#include <retesteth/TestHelper.h>
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
    string const sport = _ip.asString().substr(pos + 1);
    int port = atoi(sport.c_str());

    string trim_address = address;
    trim_address.erase(
        std::remove_if(trim_address.begin(), trim_address.end(), [](unsigned char ch) {
            return ch == '.';
        }), trim_address.end());

    if (test::stringIntegerType(trim_address) == DigitsType::String)
    {
        try
        {
            boost::asio::io_service io_service;
            boost::asio::ip::tcp::resolver resolver(io_service);
            boost::asio::ip::tcp::resolver::query query(address, "80");
            boost::asio::ip::tcp::resolver::iterator iter = resolver.resolve(query);
            boost::asio::ip::tcp::endpoint endpoint = iter->endpoint();
            address = endpoint.address().to_string();
            m_data = address + ":" + sport;
        }
        catch (std::exception const& _ex) {
            throw EthError() << string("Host `" + _ip.asString() + "` resolution failed: ") + _ex.what();
        }
    }

    boost::system::error_code ec;
    boost::asio::ip::address::from_string(address, ec);
    if (port <= 1024 || port > 49151 || ec)
        throw EthError() << "Key `" + _ip.getKey() + "` is not IPADDRESS:PORT `" + _ip.asString() + "`";
}

}  // namespace teststruct
}  // namespace test
