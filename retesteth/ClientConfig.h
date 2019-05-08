#pragma once
#include <retesteth/Socket.h>
#include <retesteth/ethObjects/object.h>
#include <boost/asio.hpp>
#include <mutex>
#include <string>
namespace fs = boost::filesystem;

namespace
{
bool validateIP(std::string const& _ip)
{
    size_t pos = _ip.find_last_of(':');
    string address = _ip.substr(0, pos);
    int port = atoi(_ip.substr(pos + 1).c_str());
    boost::system::error_code ec;
    boost::asio::ip::address::from_string(address, ec);
    if (port <= 1024 || port > 49151 || ec)
        return false;
    return true;
}
}

namespace test
{

struct ClientConfigID
{
    /// ClientConfigID handles the unique id logic so not to store it inside int and accedentially change
    /// or mistake with some other value. ???possibly a class for unique object ids???
    ClientConfigID();
    bool operator == (ClientConfigID const& _right) const { return m_id == _right.id(); }
    bool operator != (ClientConfigID const& _right) const { return m_id != _right.id(); }
    unsigned id() const { return m_id; }
private:
    unsigned m_id;
};

class ClientConfig : public object
{
public:
    ClientConfig(DataObject const& _obj, ClientConfigID const& _id, fs::path _shell = fs::path())
      : object(_obj), m_shellPath(_shell), m_id(_id)
    {
        requireJsonFields(_obj, "ClientConfig ",
            {{"name", {DataType::String}}, {"socketType", {DataType::String}},
                {"socketAddress", {DataType::String}}});
        std::string const& socketTypeStr = _obj.at("socketType").asString();
        if (socketTypeStr == "ipc")
        {
            ETH_FAIL_REQUIRE_MESSAGE(
                getAddress() == "local", "A client socket of type ipc must be deployed locally!");
            m_socketType = Socket::SocketType::IPC;
            ETH_FAIL_REQUIRE_MESSAGE(fs::exists(_shell),
                std::string("Client shell script not found: ") + _shell.c_str());
        }
        else if (socketTypeStr == "tcp")
        {
            ETH_FAIL_REQUIRE_MESSAGE(validateIP(getAddress()) == true,
                "A client tcp socket must be a correct ipv4 address!");
            m_socketType = Socket::SocketType::TCP;
        }
        else if (socketTypeStr == "ipc-debug")
        {
            m_socketType = Socket::SocketType::IPCDebug;
            ETH_FAIL_REQUIRE_MESSAGE(fs::exists(getAddress()),
                std::string("Client IPC socket file not found: ") + getAddress());
        }
        else
            ETH_FAIL_MESSAGE(
                "Incorrect client socket type: " + socketTypeStr + " in client named '" +
                getName() +
                "' Allowed socket configs [type, \"address\"]: [ipc, \"local\"], [ipc-debug, "
                "\"path to .ipc socket\"], [tcp, \"address:port\"]");
    }

    fs::path const& getShellPath() const { return m_shellPath; }
    std::string const& getName() const { return m_data.at("name").asString(); }
    Socket::SocketType getType() const { return m_socketType; }
    std::string const& getAddress() const { return m_data.at("socketAddress").asString(); }
    ClientConfigID const& getId() const { return m_id; }

private:
    Socket::SocketType m_socketType;
    fs::path m_shellPath;
    ClientConfigID m_id;
};
}
