#pragma once
#include <retesteth/testStructures/basetypes/VALUE.h>
#include <retesteth/testStructures/configs/ClientConfigFile.h>
#include <retesteth/testStructures/configs/FORK.h>
#include <string>

namespace test
{
using namespace test::teststruct;
using namespace dataobject;

struct ClientConfigID
{
    /// ClientConfigID handles the unique id logic so not to store it inside int and accedentially
    /// change or mistake with some other value. ???possibly a class for unique object ids???
    ClientConfigID();
    bool operator==(ClientConfigID const& _right) const { return m_id == _right.id(); }
    bool operator!=(ClientConfigID const& _right) const { return m_id != _right.id(); }
    unsigned id() const { return m_id; }
    static ClientConfigID null()
    {
        static ClientConfigID id;
        return id;
    }

private:
    unsigned m_id;
};

// Replace fields according to the client configs if needed
enum class FieldReplaceDir
{
    ClientToRetesteth,
    RetestethToClient
};

class ClientConfig
{
public:
    ClientConfig(boost::filesystem::path const& _clientConfigPath);
    ClientConfigID const& getId() const { return m_id; }

    // Main config file
    ClientConfigFile const& cfgFile() const { return m_clientConfigFile; }

    // Path to name.sh file for IPC client initialization
    boost::filesystem::path const getShellPath() const { return cfgFile().shell(); }
    boost::filesystem::path const getConfigPath() const { return cfgFile().path(); }

    // Functionality
    // Verify FORK is allowed by Fork + AdditionalForks and throw an error if not
    bool validateForkAllowed(FORK const& _net, bool _bail = true) const;
    bool checkForkAllowed(FORK const& _net) const;
    bool checkForkInProgression(FORK const& _net) const;

    // Translate smart network names into network names ( `<=Homestead` to `Frontier, Homestead`)
    std::vector<FORK> translateNetworks(std::set<std::string> const& _networks) const;
    static void translateNetworks(
        std::set<std::string> const& _networks, std::vector<FORK> const& _netOrder, std::vector<FORK>& _out);

    // Translate exceptionID from tests into client error string from configs
    // Print suggestions if no match found
    std::string const& translateException(std::string const& _exceptionName) const;

    // Get Contents of genesis template for specified FORK
    spDataObject const& getGenesisTemplate(FORK const& _fork) const;
    std::map<FORK, spVALUE> const getGenesisTemplateChainID() const { return m_genesisTemplateChainID; }

    // Get reward information info for each fork
    std::map<FORK, spVALUE> const& getRewardMap() const { return m_correctReward; }

    // Get path to correct mining reward info file
    boost::filesystem::path const& getRewardMapPath() const { return m_correctMiningRewardPath; }

    // Get path to scripts
    void initializeFirstSetup();
    boost::filesystem::path const& getSetupScript() const { return m_setupScriptPath; }
    boost::filesystem::path const& getStartScript() const { return m_starterScriptPath; }
    boost::filesystem::path const& getStopperScript() const { return m_stopperScriptPath; }

    // Replace notations in requests if needed
    void performFieldReplace(DataObject& _data, FieldReplaceDir const& _dir) const;

private:
    ClientConfigID m_id;                                ///< Internal id
    GCP_SPointer<ClientConfigFile> m_clientConfigFile;  ///< <clientname>/config file
    std::map<FORK, spVALUE> m_correctReward;            ///< Correct mining reward info for StateTests->BlockchainTests
    std::map<FORK, spDataObject> m_genesisTemplate;     ///< Template For test_setChainParams
    std::map<FORK, spVALUE> m_genesisTemplateChainID;   ///< ChainID value from template read


    boost::filesystem::path m_correctMiningRewardPath;  ///< Path to correct mining reward info file

    bool m_initialized = false;    ///< If setup script has run
    boost::filesystem::path m_setupScriptPath;    ///< Path to setup script (run once before thread exec)
    boost::filesystem::path m_starterScriptPath;  ///< Path to starter script
    boost::filesystem::path m_stopperScriptPath;  ///< Path to stopper script
};

}  // namespace test
