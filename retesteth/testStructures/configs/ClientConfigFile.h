#pragma once
#include "../basetypes/IPADDRESS.h"
#include "FORK.h"
#include <libdataobj/DataObject.h>
#include <libdataobj/SPointer.h>
#include <boost/filesystem.hpp>
#include <map>

namespace test::teststruct
{
namespace fs = boost::filesystem;
enum class ClientConfgSocketType
{
    TCP,
    IPC,
    IPCDebug,
    TransitionTool
};

struct ClientConfigFile : GCP_SPointerBase
{
    ClientConfigFile(DataObject const& _data);
    ClientConfigFile(fs::path const& _clientConfigPath);

    std::string const& name() const { return m_name; }
    ClientConfgSocketType socketType() const { return m_socketType; }
    std::vector<IPADDRESS> const& socketAdresses() const;
    std::map<std::string, fs::path> const& customCompilers() const { return m_customCompilers; }
    size_t initializeTime() const { return  m_initializeTime; }
    std::vector<FORK> const& forks() const { return m_forks; }
    std::vector<FORK> const& additionalForks() const { return m_additionalForks; }
    std::set<FORK> allowedForks() const;
    std::set<FORK> forkProgressionAsSet() const;
    bool checkLogsHash() const { return m_checkLogsHash; }
    int chainID() const { return m_chanID; }

    std::map<std::string, std::string> const& exceptions() const { return m_exceptions; }
    std::map<std::string, std::string> const& fieldreplace() const { return m_fieldRaplce; }
    fs::path const& path() const { return m_configFilePath; }
    fs::path const& shell() const { return m_pathToExecFile; }


private:
    ClientConfigFile() {}
    void initWithData(DataObject const&);
    void parseSocketType(DataObject const& _data, std::string const& _sErrorPath);

private:
    // Inside the file
    std::string m_name;                      ///< Client name
    ClientConfgSocketType m_socketType;      ///< Connection type
    std::vector<IPADDRESS> m_socketAddress;  ///< List of IP to connect to (IP::PORT)

    std::map<std::string, fs::path> m_customCompilers;
    bool m_checkLogsHash;                    ///< Enable logsHash verification
    int m_chanID;                            ///< Use custom chainID

    size_t m_initializeTime;                 ///< Time to start the instance
    std::vector<FORK> m_forks;               ///< Allowed forks as network name
    std::vector<FORK> m_additionalForks;     ///< Allowed forks as network name
    std::map<std::string, std::string> m_exceptions;   ///< Exception Translation
    std::map<std::string, std::string> m_fieldRaplce;  ///< Replace field names in requests map

    // Additional values
    fs::path m_configFilePath;  ///< Path to the config file
    fs::path m_pathToExecFile;  ///< Path to cmd that runs the client instance (for t8ntool)
};


}  // namespace teststruct
