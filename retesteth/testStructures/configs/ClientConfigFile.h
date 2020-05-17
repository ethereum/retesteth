#pragma once
#include "../basetypes/IPADDRESS.h"
#include "FORK.h"
#include <retesteth/dataObject/DataObject.h>
#include <retesteth/dataObject/SPointer.h>
#include <session/Socket.h>
#include <boost/filesystem.hpp>
using namespace dataobject;
namespace fs = boost::filesystem;

namespace test
{
namespace teststruct
{
struct ClientConfigFile : GCP_SPointerBase
{
    ClientConfigFile(DataObject const& _data);
    ClientConfigFile(fs::path const& _clientConfigPath);

    string const& name() const { return m_name; }
    Socket::SocketType socketType() const { return m_socketType; }
    std::vector<IPADDRESS> const& socketAdresses() const { return m_socketAddress; }
    std::vector<FORK> const& forks() const { return m_forks; }
    std::vector<FORK> const& additionalForks() const { return m_additionalForks; }
    std::set<FORK> allowedForks() const
    {
        std::set<FORK> out;
        for (auto const& el : m_forks)
            out.insert(el);
        for (auto const& el : m_additionalForks)
            out.insert(el);
        return out;
    }
    std::map<string, string> const& exceptions() const { return m_exceptions; }


private:
    ClientConfigFile() {}
    void initWithData(DataObject const&);

    // Inside the file
    string m_name;                           ///< Client name
    Socket::SocketType m_socketType;         ///< Connection type
    std::vector<IPADDRESS> m_socketAddress;  ///< List of IP to connect to (IP::PORT)
    std::vector<FORK> m_forks;               ///< Allowed forks as network name
    std::vector<FORK> m_additionalForks;     ///< Allowed forks as network name
    std::map<string, string> m_exceptions;   ///< Exception Translation

    // Additional values
    fs::path m_configFilePath;  ///< Path to the config file
    fs::path m_pathToExecFile;  ///< Path to cmd that runs the client instance (for t8ntool)

    // fs::path m_shellPath;             ///< Script to start new instance of a client (for ipc)
    // fs::path m_configFilePath;        ///< Path to the client fork networks config
    // fs::path m_configCorrectMiningRewardFilePath;    ///< Config correctMiningReward file path
    // ClientConfigID m_id;                             ///< Internal id
    // std::map<string, DataObject> m_genesisTemplate;  ///< Template For test_setChainParams
    // std::map<string, string> m_exceptions;
    // std::string m_folderName;                        ///< Config folder name
    // DataObject m_correctReward;  ///< Correct mining reward info for StateTests->BlockchainTests
};


}  // namespace teststruct
}  // namespace test
