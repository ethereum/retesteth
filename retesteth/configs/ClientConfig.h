#pragma once
#include <retesteth/TestHelper.h>

#include <retesteth/testStructures/basetypes/VALUE.h>
#include <retesteth/testStructures/configs/ClientConfigFile.h>
#include <retesteth/testStructures/configs/FORK.h>

#include <boost/asio.hpp>
#include <mutex>
#include <string>
namespace fs = boost::filesystem;
using namespace test::teststruct;

namespace test
{
struct ClientConfigID
{
    /// ClientConfigID handles the unique id logic so not to store it inside int and accedentially
    /// change or mistake with some other value. ???possibly a class for unique object ids???
    ClientConfigID();
    bool operator==(ClientConfigID const& _right) const { return m_id == _right.id(); }
    bool operator!=(ClientConfigID const& _right) const { return m_id != _right.id(); }
    unsigned id() const { return m_id; }

private:
    unsigned m_id;
};

class ClientConfig
{
public:
    // ???
    DataObject const& getGenesisTemplate(string const& _network) const
    {
        ETH_FAIL_REQUIRE_MESSAGE(m_genesisTemplate.count(FORK(_network)),
            "Genesis template for network '" + _network + "' not found!");
        return m_genesisTemplate.at(FORK(_network));
    }


    ClientConfig(fs::path const& _clientConfigPath);

    // Refactoring
    ClientConfigFile const& cfgFile() const { return *m_clientConfigFile.getCPtr(); }
    ClientConfigID const& getId() const { return m_id; }
    fs::path const getShellPath() const;

    // Functionality
    // Verify FORK is allowed by Fork + AdditionalForks and throw an error if not
    void checkForkAllowed(FORK const& _net) const;

    // Translate smart network names into network names ( `<=Homestead` to `Frontier, Homestead`)
    set<FORK> translateNetworks(set<string> const& _networks) const;

    // Translate exceptionID from tests into client error string from configs
    // Print suggestions if no match found
    std::string const& translateException(string const& _exceptionName) const;


private:
    ClientConfigID m_id;                                ///< Internal id
    GCP_SPointer<ClientConfigFile> m_clientConfigFile;  ///< <clientname>/config file
    std::map<FORK, spVALUE> m_correctReward;            ///< Correct mining reward info for
                                                        ///< StateTests->BlockchainTests
    std::map<FORK, DataObject> m_genesisTemplate;       ///< Template For test_setChainParams
};

}  // namespace test
