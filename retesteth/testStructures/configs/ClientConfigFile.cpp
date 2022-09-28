#include "ClientConfigFile.h"
#include <testStructures/Common.h>
#include <retesteth/EthChecks.h>
#include <retesteth/TestHelper.h>
using namespace std;
using namespace test::teststruct;
std::mutex g_allowedForks_static_var;
std::mutex g_forkProgressionAsSet_static_var;

namespace
{
void requireJsonFileStructure(DataObject const& _data)
{
    // Limit sections in the file
    REQUIRE_JSONFIELDS(_data, "ClientConfigFile " + _data.getKey(),
        {{"name", {{DataType::String}, jsonField::Required}}, {"socketType", {{DataType::String}, jsonField::Required}},
            {"socketAddress", {{DataType::String, DataType::Array}, jsonField::Required}},
            {"customCompilers", {{DataType::Object}, jsonField::Optional}},
            {"initializeTime", {{DataType::String}, jsonField::Optional}},
            {"checkLogsHash", {{DataType::Bool}, jsonField::Optional}}, {"chainID", {{DataType::Integer}, jsonField::Optional}},
            {"forks", {{DataType::Array}, jsonField::Required}}, {"additionalForks", {{DataType::Array}, jsonField::Required}},
            {"exceptions", {{DataType::Object}, jsonField::Required}},
            {"fieldReplace", {{DataType::Object}, jsonField::Optional}}});
}
}  // namespace

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

void ClientConfigFile::parseSocketType(DataObject const& _data, string const& _sErrorPath)
{
    // SocketTypes for client connection
    std::string const& socketTypeStr = _data.atKey("socketType").asString();
    if (socketTypeStr == "ipc")
        m_socketType = ClientConfgSocketType::IPC;
    else if (socketTypeStr == "tcp")
        m_socketType = ClientConfgSocketType::TCP;
    else if (socketTypeStr == "ipc-debug")
        m_socketType = ClientConfgSocketType::IPCDebug;
    else if (socketTypeStr == "tranition-tool")
        m_socketType = ClientConfgSocketType::TransitionTool;
    else
        ETH_FAIL_MESSAGE(_sErrorPath + "Unknown `socketType` : " + socketTypeStr +
                         ", Allowed: ['ipc', 'tcp', 'ipc-debug', 'transition-tool']");

    // SocketAddress is an array of ipaddresses or path to a socket file
    if (m_socketType == ClientConfgSocketType::TCP)
    {
        if (_data.atKey("socketAddress").getSubObjects().size() == 0)
            ETH_FAIL_MESSAGE(_sErrorPath + "socketAddress must be non empty array!");
        for (auto const& el : _data.atKey("socketAddress").getSubObjects())
        {
            IPADDRESS addr(el);
            if (test::inArray(m_socketAddress, addr))
                ETH_ERROR_MESSAGE(_sErrorPath + "`socketAddress` section contain dublicate element: " + el->asString());
            m_socketAddress.push_back(addr);
        }
    }
    // IPC would connect to a client using shell script
    else if (m_socketType == ClientConfgSocketType::IPC)
    {
        if (_data.atKey("socketAddress").type() != DataType::String)
            ETH_FAIL_MESSAGE(_sErrorPath + "`socketAddress` must be string for this socketType!");
        m_pathToExecFile = fs::path(_data.atKey("socketAddress").asString());
        if (!fs::exists(m_pathToExecFile))
            m_pathToExecFile = m_configFilePath.parent_path() / m_pathToExecFile;  // try relative path
        if (!fs::exists(m_pathToExecFile))
            ETH_FAIL_MESSAGE(_sErrorPath +
                             "`socketAddress` for socketType::ipc must point to a shell script, "
                             "that runs a client instance! " +
                             "But file `" + m_pathToExecFile.string() + "` not found!");
    }
    // IPCDebug would connect to already running unix socket
    else if (m_socketType == ClientConfgSocketType::IPCDebug)
    {
        if (_data.atKey("socketAddress").type() != DataType::String)
            ETH_FAIL_MESSAGE(_sErrorPath + "`socketAddress` must be string for this socketType!");
        m_pathToExecFile = fs::path(_data.atKey("socketAddress").asString());
        if (!fs::exists(m_pathToExecFile))
            ETH_FAIL_MESSAGE(_sErrorPath + "`socketAddress` for socketType::ipc-debug must point to a running unix socket!" +
                             " But socket file not found (" + m_pathToExecFile.string() + ")");
    }
    // Transition tool would not use Socket class and run tool file instead.
    else if (m_socketType == ClientConfgSocketType::TransitionTool)
    {
        if (_data.atKey("socketAddress").type() != DataType::String)
            ETH_FAIL_MESSAGE(_sErrorPath + "`socketAddress` must be string for this socketType!");

        m_pathToExecFile = fs::path(_data.atKey("socketAddress").asString());
        fs::path const cfgPath = m_configFilePath.parent_path();
        ETH_FAIL_REQUIRE_MESSAGE(fs::exists(m_pathToExecFile) || fs::exists(cfgPath / m_pathToExecFile),
            _sErrorPath + "`socketAddress` for socketType::transition-tool must point to a tool cmd!" +
                " But file not found (" + m_pathToExecFile.string() + ")");
        if (fs::exists(cfgPath / m_pathToExecFile))
            m_pathToExecFile = cfgPath / m_pathToExecFile;
    }
}

void ClientConfigFile::initWithData(DataObject const& _data)
{
    requireJsonFileStructure(_data);

    string const sErrorPath = "ClientConfig (" + m_configFilePath.string() + ") ";
    m_name = _data.atKey("name").asString();

    parseSocketType(_data, sErrorPath);

    m_initializeTime = 0;
    if (_data.count("initializeTime"))
        m_initializeTime = atoi(_data.atKey("initializeTime").asString().c_str());

    m_checkLogsHash = false;
    if (_data.count("checkLogsHash"))
        m_checkLogsHash = _data.atKey("checkLogsHash").asBool();

    m_chanID = 1;
    if (_data.count("chainID"))
        m_chanID = _data.atKey("chainID").asInt();

    // Read forks as fork order. Order is required for translation (`>=Frontier` -> `Frontier,
    // Homestead`) According to this order:
    for (auto const& el : _data.atKey("forks").getSubObjects())
    {
        if (el->asString()[0] == '/' && el->asString()[1] == '/')
            continue;
        if (test::inArray(m_forks, FORK(el)))
            ETH_ERROR_MESSAGE(sErrorPath + "`forks` section contain dublicate element: " + el->asString());
        m_forks.push_back(FORK(el));
    }

    // Read additionalForks are allowed fork names to run on this client, but not used in translation
    for (auto const& el : _data.atKey("additionalForks").getSubObjects())
    {
        if (el->asString()[0] == '/' && el->asString()[1] == '/')
            continue;
        if (test::inArray(m_additionalForks, FORK(el)))
            ETH_ERROR_MESSAGE(sErrorPath + "`additionalForks` section contain dublicate element: " + el->asString());
        m_additionalForks.push_back(FORK(el));
    }

    // When client used to fill up tests this map translate exceptionsID in test to exception string
    // returned from client
    for (auto const& el : _data.atKey("exceptions").getSubObjects())
    {
        if (m_exceptions.count(el->getKey()))
            ETH_ERROR_MESSAGE(sErrorPath + "`exceptions` section contain dublicate element: " + el->getKey());
        m_exceptions[el->getKey()] = el->asString();
    }

    // When sending requests to the client, some of the parameters might be named differently
    // To be flexible here is the replacement map that will change retesteth format field names to client format names
    if (_data.count("fieldReplace"))
    {
        for (auto const& el : _data.atKey("fieldReplace").getSubObjects())
        {
            if (m_fieldRaplce.count(el->getKey()))
                ETH_ERROR_MESSAGE(sErrorPath + "`fieldReplace` section contain dublicate element: " + el->getKey());
            m_fieldRaplce[el->getKey()] = el->asString();
        }
    }

    if (_data.count("customCompilers"))
    {
        auto const& map = _data.atKey("customCompilers");
        for (auto const& compiler : map.getSubObjects())
        {
            ETH_ERROR_REQUIRE_MESSAGE(compiler->getKey()[0] == ':',
                sErrorPath + "`customCompilers` keyword must begin with `:` " + compiler->getKey());
            ETH_ERROR_REQUIRE_MESSAGE(compiler->type() == DataType::String,
                sErrorPath + "`customCompilers` section must has map struture <string> : <string>");
            fs::path const cfgPath = m_configFilePath.parent_path();
            ETH_FAIL_REQUIRE_MESSAGE(fs::exists(compiler->asString()) || fs::exists(cfgPath / compiler->asString()),
                sErrorPath + "`customCompilers` element must point to executable cmd!" + " But file not found (" +
                    compiler->asString() + ")");

            if (fs::exists(cfgPath / compiler->asString()))
                m_customCompilers.emplace(compiler->getKey(), cfgPath / compiler->asString());
            else
                m_customCompilers.emplace(compiler->getKey(), compiler->asString());
        }
    }
}

std::vector<IPADDRESS> const& ClientConfigFile::socketAdresses() const
{
    if (m_socketType != ClientConfgSocketType::TCP)
        ETH_FAIL_MESSAGE("Attempt to geth addresses of ClientConfig which is not TCP socket type!");
    return m_socketAddress;
}

std::set<FORK> ClientConfigFile::allowedForks() const
{
    std::lock_guard<std::mutex> lock(g_allowedForks_static_var);
    static std::set<FORK> out;
    if (out.size() == 0)
    {
        for (auto const& el : m_forks)
            out.insert(el);
        for (auto const& el : m_additionalForks)
            out.insert(el);
    }
    return out;
}

std::set<FORK> ClientConfigFile::forkProgressionAsSet() const
{
    std::lock_guard<std::mutex> lock(g_forkProgressionAsSet_static_var);
    static std::set<FORK> out;
    if (out.size() == 0)
    {
        for (auto const& el : m_forks)
            out.insert(el);
    }
    return out;
}


}  // namespace teststruct
}  // namespace test
