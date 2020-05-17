#include "ClientConfigFile.h"
#include "../basetypes/IPADDRESS.h"
#include <retesteth/EthChecks.h>
#include <retesteth/TestHelper.h>
using namespace test::teststruct;

namespace test
{
namespace teststruct
{
ClientConfigFile::ClientConfigFile(DataObject const& _data)
{
    initWithData(_data);
}

ClientConfigFile::ClientConfigFile(fs::path const& _clientConfigPath)
{
    m_configFilePath = _clientConfigPath;
    initWithData(test::readJsonData(m_configFilePath));
}

void ClientConfigFile::initWithData(DataObject const& _data)
{
    string const sErrorPath = "ClientConfig (" + m_configFilePath.string() + ") ";
    m_name = _data.atKey("name").asString();

    // SocketTypes for client connection
    std::string const& socketTypeStr = _data.atKey("socketType").asString();
    if (socketTypeStr == "ipc")
        m_socketType = Socket::SocketType::IPC;
    else if (socketTypeStr == "tcp")
        m_socketType = Socket::SocketType::TCP;
    else if (socketTypeStr == "ipc-debug")
        m_socketType = Socket::SocketType::IPCDebug;
    else if (socketTypeStr == "tranition-tool")
        m_socketType = Socket::SocketType::TransitionTool;
    else
        ETH_FAIL_MESSAGE(sErrorPath + "Unknown `socketType` : " + socketTypeStr +
                         ", Allowed: ['ipc', 'tcp', 'ipc-debug', 'transition-tool']");

    // SocketAddress is an array of ipaddresses or path to a socket file
    // IPC would connect to a client using shell script
    if (m_socketType != Socket::SocketType::IPC)
    {
        if (_data.atKey("socketAddress").type() == DataType::String)
        {
            m_pathToExecFile = fs::path(_data.atKey("socketAddress").asString());
            ETH_FAIL_REQUIRE_MESSAGE(fs::exists(m_pathToExecFile),
                "Provided socketPath doesn't exist: " + m_pathToExecFile.string());
        }
        else
        {
            for (auto const& el : _data.atKey("socketAddress").getSubObjects())
            {
                IPADDRESS addr(el);
                if (test::inArray(m_socketAddress, addr))
                    ETH_ERROR_MESSAGE(
                        sErrorPath +
                        "`socketAddress` section contain dublicate element: " + el.asString());
                m_socketAddress.push_back(addr);
            }
        }
    }

    // Read forks as fork order. Order is required for translation (`>=Frontier` -> `Frontier,
    // Homestead`) According to this order:
    for (auto const& el : _data.atKey("forks").getSubObjects())
    {
        if (test::inArray(m_forks, FORK(el)))
            ETH_ERROR_MESSAGE(
                sErrorPath + "`forks` section contain dublicate element: " + el.asString());
        m_forks.push_back(FORK(el));
    }

    // Read additionalForks are allowed fork names to run on this client, but not used in
    // translation
    for (auto const& el : _data.atKey("additionalForks").getSubObjects())
    {
        if (test::inArray(m_additionalForks, FORK(el)))
            ETH_ERROR_MESSAGE(sErrorPath + "`additionalForks` section contain dublicate element: " +
                              el.asString());
        m_additionalForks.push_back(FORK(el));
    }

    // When client used to fill up tests this map translate exceptionsID in test to exception string
    // returned from client
    for (auto const& el : _data.atKey("exceptions").getSubObjects())
    {
        if (m_exceptions.count(el.getKey()))
            ETH_ERROR_MESSAGE(
                sErrorPath + "`exceptions` section contain dublicate element: " + el.getKey());
        m_exceptions[el.getKey()] = el.asString();
    }
}

}  // namespace teststruct
}  // namespace test
