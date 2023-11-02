#include "ClientConfigFile.h"
#include <testStructures/Common.h>
#include <retesteth/EthChecks.h>
#include <retesteth/helpers/TestHelper.h>
using namespace std;
using namespace test::teststruct;
namespace fs = boost::filesystem;

namespace
{
void requireJsonFileStructure(DataObject const& _data)
{
    // Limit sections in the file
    REQUIRE_JSONFIELDS(_data, "ClientConfigFile " + _data.getKey(),
        {{"name", {{DataType::String}, jsonField::Required}},
            {"socketType", {{DataType::String}, jsonField::Required}},
            {"socketAddress", {{DataType::String, DataType::Array}, jsonField::Required}},
            {"customCompilers", {{DataType::Object}, jsonField::Optional}},
            {"initializeTime", {{DataType::String}, jsonField::Optional}},
            {"tmpDir", {{DataType::String}, jsonField::Optional}},
            {"transactionsAsJson", {{DataType::Bool}, jsonField::Optional}},
            {"checkLogsHash", {{DataType::Bool}, jsonField::Optional}},
            {"checkDifficulty", {{DataType::Bool}, jsonField::Optional}},
            {"calculateDifficulty", {{DataType::Bool}, jsonField::Optional}},
            {"support1559", {{DataType::Bool}, jsonField::Optional}},
            {"supportBigint", {{DataType::Bool}, jsonField::Optional}},
            {"checkBasefee", {{DataType::Bool}, jsonField::Optional}},
            {"calculateBasefee", {{DataType::Bool}, jsonField::Optional}},
            {"defaultChainID", {{DataType::Integer}, jsonField::Optional}},
            {"continueOnErrors", {{DataType::Bool}, jsonField::Optional}},
            {"forks", {{DataType::Array}, jsonField::Required}},
            {"additionalForks", {{DataType::Array}, jsonField::Required}},
            {"fillerSkipForks", {{DataType::Array}, jsonField::Optional}},
            {"exceptions", {{DataType::Object}, jsonField::Required}},
            {"fieldReplace", {{DataType::Object}, jsonField::Optional}}});
}
}  // namespace

namespace test::teststruct
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
            m_socketAddress.emplace_back(addr);
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

    m_defaultChainID = 1;
    if (_data.count("defaultChainID"))
        m_defaultChainID = _data.atKey("defaultChainID").asInt();

    m_checkLogsHash = false;
    if (_data.count("checkLogsHash"))
        m_checkLogsHash = _data.atKey("checkLogsHash").asBool();

    m_checkDifficulty = true;
    if (_data.count("checkDifficulty"))
        m_checkDifficulty = _data.atKey("checkDifficulty").asBool();

    m_calculateDifficulty = false;
    if (_data.count("calculateDifficulty"))
        m_calculateDifficulty = _data.atKey("calculateDifficulty").asBool();

    m_calculateBasefee = false;
    if (_data.count("calculateBasefee"))
        m_calculateBasefee = _data.atKey("calculateBasefee").asBool();

    m_checkBasefee = false;
    if (_data.count("checkBasefee"))
        m_checkBasefee = _data.atKey("checkBasefee").asBool();

    m_support1559 = true;
    if (_data.count("support1559"))
        m_support1559 = _data.atKey("support1559").asBool();

    m_supportBigint = true;
    if (_data.count("supportBigint"))
        m_supportBigint = _data.atKey("supportBigint").asBool();

    m_transactionsAsJson = false;
    if (_data.count("transactionsAsJson"))
        m_transactionsAsJson = _data.atKey("transactionsAsJson").asBool();

    m_continueOnErrors = false;
    if (_data.count("continueOnErrors"))
        m_continueOnErrors = _data.atKey("continueOnErrors").asBool();

    if (_data.count("tmpDir"))
    {
        string const& tpath = _data.atKey("tmpDir").asString();
        m_tmpDir = fs::path(tpath);
        if (!fs::exists(m_tmpDir))
        {
            ETH_WARNING(sErrorPath + "tmpDir ('" + tpath + "') location not found! Switching to default tmpDir location (`/dev/shm` or boost tmp path)");
            m_tmpDir = "";
        }
    }

    // Read forks as fork order. Order is required for translation (`>=Frontier` -> `Frontier,
    // Homestead`) According to this order:
    for (auto const& el : _data.atKey("forks").getSubObjects())
    {
        if (el->asString()[0] == '/' && el->asString()[1] == '/')
            continue;
        FORK const fork(el);
        if (test::inArray(m_forks, fork))
            ETH_ERROR_MESSAGE(sErrorPath + "`forks` section contain dublicate element: " + el->asString());
        m_forks.emplace_back(fork);
        m_forkProgressionAsSet.insert(fork);
    }

    // Read additionalForks are allowed fork names to run on this client, but not used in translation
    for (auto const& el : _data.atKey("additionalForks").getSubObjects())
    {
        if (el->asString()[0] == '/' && el->asString()[1] == '/')
            continue;
        if (test::inArray(m_additionalForks, FORK(el)))
            ETH_ERROR_MESSAGE(sErrorPath + "`additionalForks` section contain dublicate element: " + el->asString());
        m_additionalForks.emplace_back(FORK(el));
    }

    // Read fillerSkipForks are allowed to skip fork names when filling the test
    if (_data.count("fillerSkipForks"))
    {
        for (auto const& el : _data.atKey("fillerSkipForks").getSubObjects())
        {
            if (el->asString()[0] == '/' && el->asString()[1] == '/')
                continue;
            if (test::inArray(m_skipForks, FORK(el)))
                ETH_ERROR_MESSAGE(sErrorPath + "`fillerSkipForks` section contain dublicate element: " + el->asString());
            m_skipForks.emplace_back(FORK(el));
        }
    }

    // Build allowed forks
    for (auto const& el : m_forks)
        m_allowedForks.insert(el);
    for (auto const& el : m_additionalForks)
        m_allowedForks.insert(el);
    for (auto const& el : m_skipForks)
        m_allowedForks.insert(el);

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

std::set<FORK> const& ClientConfigFile::allowedForks() const
{
    return m_allowedForks;
}

std::set<FORK> const& ClientConfigFile::forkProgressionAsSet() const
{
    return m_forkProgressionAsSet;
}


}  // namespace teststruct
