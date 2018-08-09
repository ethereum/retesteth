#pragma once
#include <retesteth/Socket.h>
#include <retesteth/ethObjects/object.h>
#include <boost/asio.hpp>
#include <string>
namespace fs = boost::filesystem;

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
    ClientConfig(DataObject const& _obj, unsigned _id, fs::path _shell = fs::path())
      : object(_obj), m_shellPath(_shell), m_id(_id)
    {
        requireJsonFields(_obj, "ClientConfig ",
            {{"name", {DataType::String}}, {"socketType", {DataType::String}},
                {"socketAddress", {DataType::String}}});
        std::string const& socketTypeStr = _obj.at("socketType").asString();
        if (socketTypeStr == "ipc")
        {
            ETH_REQUIRE_MESSAGE(
                getAddress() == "local", "A client socket of type ipc must be deployed locally!");
            m_socketType = Socket::SocketType::IPC;
            ETH_REQUIRE_MESSAGE(fs::exists(_shell),
                std::string("Client shell script not found: ") + _shell.c_str());
        }
        else if (socketTypeStr == "tcp")
        {
            ETH_REQUIRE_MESSAGE(validateIP(getAddress()) == true,
                "A client tcp socket must be a correct ipv4 address!");
            m_socketType = Socket::SocketType::TCP;
        }
        else
            ETH_FAIL("Incorrect client socket type: " + socketTypeStr + " in client named '" +
                     getName() + "'");
    }

    fs::path const& getShellPath() const { return m_shellPath; }
    std::string const& getName() const { return m_data.at("name").asString(); }
    Socket::SocketType getType() const { return m_socketType; }
    std::string const& getAddress() const { return m_data.at("socketAddress").asString(); }
    unsigned getId() const { return m_id; }

private:
    Socket::SocketType m_socketType;
    fs::path m_shellPath;
    unsigned m_id;
};
}
