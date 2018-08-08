#pragma once
#include <retesteth/ethObjects/object.h>
#include <boost/asio.hpp>
#include <string>
namespace
{
bool validateIP(std::string const& _ip)
{
    boost::system::error_code ec;
    boost::asio::ip::address::from_string(_ip, ec);
    if (ec)
        return false;
    return true;
}
}

namespace test
{
class ClientConfig : public object
{
public:
    ClientConfig(DataObject const& _obj) : object(_obj)
    {
        requireJsonFields(_obj, "ClientConfig ",
            {{"name", {DataType::String}}, {"socketType", {DataType::String}},
                {"socketAddress", {DataType::String}}});
        if (getType() == "ipc")
            ETH_REQUIRE_MESSAGE(
                getAddress() == "local", "A client socket of type ipc must be deployed locally!");
        else if (getType() == "tcp")
            ETH_REQUIRE_MESSAGE(validateIP(getAddress()) == true,
                "A client tcp socket must be a correct ipv4 address!");
        else
            ETH_FAIL("Incorrect client socket type: " + getType() + " in client named '" +
                     getName() + "'");
    }

    std::string const& getName() { return m_data.at("name").asString(); }
    std::string const& getType() { return m_data.at("socketType").asString(); }
    std::string const& getAddress() { return m_data.at("socketAddress").asString(); }
};
}
