#pragma once
#include "../basetypes/IPADDRESS.h"
#include "FORK.h"
#include <libdataobj/DataObject.h>
#include <boost/filesystem/path.hpp>
#include <map>

namespace test::teststruct
{
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
    ClientConfigFile(boost::filesystem::path const& _clientConfigPath);

    std::string const& name() const { return m_name; }
    ClientConfgSocketType socketType() const { return m_socketType; }
    std::vector<IPADDRESS> const& socketAdresses() const;
    std::map<std::string, boost::filesystem::path> const& customCompilers() const { return m_customCompilers; }
    size_t initializeTime() const { return  m_initializeTime; }
    int defaultChainID() const { return m_defaultChainID; }
    boost::filesystem::path const& tmpDir() const { return m_tmpDir; }
    std::vector<FORK> const& forks() const { return m_forks; }
    std::vector<FORK> const& additionalForks() const { return m_additionalForks; }
    std::vector<FORK> const& fillerSkipForks() const { return m_skipForks; }
    std::set<FORK> const& allowedForks() const;
    std::set<FORK> const& forkProgressionAsSet() const;
    bool checkLogsHash() const { return m_checkLogsHash; }

    bool checkDifficulty() const { return m_checkDifficulty; }
    bool calculateDifficulty() const { return m_calculateDifficulty; }

    bool checkBasefee() const { return m_checkBasefee; }
    bool calculateBasefee() const { return m_calculateBasefee; }

    // ETC classic block format autoconvertion
    bool support1559() const { return m_support1559; }
    bool supportBigint() const { return m_supportBigint; }
    bool transactionsAsJson() const { return m_transactionsAsJson; }
    bool continueOnErrors() const { return m_continueOnErrors; }

    std::map<std::string, std::string> const& exceptions() const { return m_exceptions; }
    std::map<std::string, std::string> const& fieldreplace() const { return m_fieldRaplce; }
    boost::filesystem::path const& path() const { return m_configFilePath; }
    boost::filesystem::path const& shell() const { return m_pathToExecFile; }


private:
    ClientConfigFile() {}
    void initWithData(DataObject const&);
    void parseSocketType(DataObject const& _data, std::string const& _sErrorPath);

private:
    // Inside the file
    std::string m_name;                      ///< Client name
    ClientConfgSocketType m_socketType;      ///< Connection type
    std::vector<IPADDRESS> m_socketAddress;  ///< List of IP to connect to (IP::PORT)
    int m_defaultChainID = 1;                ///< Transactions to initialize with this chainID by default

    std::map<std::string, boost::filesystem::path> m_customCompilers;
    boost::filesystem::path m_tmpDir;        ///< Path to the temp dir for this config
    bool m_checkLogsHash;                    ///< Enable logsHash verification
    bool m_checkDifficulty;                  ///< Enable difficulty verification
    bool m_calculateDifficulty;              ///< Retesteth calculate difficulty for the client
    bool m_checkBasefee;                     ///< Enable basefee verifivation
    bool m_calculateBasefee;                 ///< Retesteth calculate basefee value
    bool m_support1559;                      ///< Support EIP1559 headers
    bool m_supportBigint;                    ///< Support malicious oversize data encodings for tests
    bool m_transactionsAsJson;               ///< Make T8N txs file as json not rlp
    bool m_continueOnErrors;                 ///< Continue test run on error
    size_t m_initializeTime;                 ///< Time to start the instance
    std::vector<FORK> m_forks;               ///< Allowed forks as network name
    std::vector<FORK> m_additionalForks;     ///< Allowed forks as network name
    std::vector<FORK> m_skipForks;           ///< Allowed forks to skip when filling
    std::map<std::string, std::string> m_exceptions;   ///< Exception Translation
    std::map<std::string, std::string> m_fieldRaplce;  ///< Replace field names in requests map

    // Additional values
    boost::filesystem::path m_configFilePath;  ///< Path to the config file
    boost::filesystem::path m_pathToExecFile;  ///< Path to cmd that runs the client instance (for t8ntool)
private:
    // Optimisations
    mutable std::set<FORK> m_forkProgressionAsSet;
    mutable std::set<FORK> m_allowedForks;
};


}  // namespace teststruct
