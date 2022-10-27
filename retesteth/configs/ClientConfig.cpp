#include <retesteth/EthChecks.h>
#include <retesteth/TestHelper.h>
#include <retesteth/TestOutputHelper.h>
#include <retesteth/configs/ClientConfig.h>
#include <retesteth/testStructures/Common.h>
using namespace std;
using namespace dataobject;
using namespace test::debug;
using namespace test::teststruct;
namespace fs = boost::filesystem;

std::mutex g_staticDeclaration_clientConfigID;
std::mutex g_staticDeclaration_translateNetworks_static;
namespace test
{
ClientConfigID::ClientConfigID()
{
    std::lock_guard<std::mutex> lock(g_staticDeclaration_clientConfigID);
    static unsigned uniqueID = 0;
    uniqueID++;
    m_id = uniqueID;
}

ClientConfig::ClientConfig(fs::path const& _clientConfigPath) : m_id(ClientConfigID())
{
    try
    {
        fs::path const default_ClientConfigPath = _clientConfigPath.parent_path() / "default";
        TestOutputHelper::get().setCurrentTestInfo(TestInfo("Client Config init"));
        fs::path configFile = _clientConfigPath / "config";
        ETH_FAIL_REQUIRE_MESSAGE(fs::exists(configFile), string("Client config not found: ") + configFile.c_str());

        // Script to setup the instance
        fs::path setupScript = _clientConfigPath / "setup.sh";
        if (fs::exists(setupScript))
            m_setupScriptPath = setupScript;

        // Script to start the instance
        fs::path startScript = _clientConfigPath / "start.sh";
        if (fs::exists(startScript))
            m_starterScriptPath = startScript;

        // Script to stop the instance
        fs::path stopScript = _clientConfigPath / "stop.sh";
        if (fs::exists(stopScript))
            m_stopperScriptPath = stopScript;

        // Load client config file
        m_clientConfigFile = GCP_SPointer<ClientConfigFile>(new ClientConfigFile(configFile));

        // Load genesis templates from default dir if not set in this folder
        fs::path genesisTemplatePath = _clientConfigPath / "genesis";
        fs::path default_genesisTemplatePath = default_ClientConfigPath / "genesis";
        if (!fs::exists(genesisTemplatePath))
        {
            genesisTemplatePath = _clientConfigPath.parent_path() / "default" / "genesis";
            ETH_FAIL_REQUIRE_MESSAGE(fs::exists(genesisTemplatePath), "default/genesis client config not found!");
        }

        // Load genesis templates
        for (auto const& net : cfgFile().allowedForks())
        {
            fs::path const configGenesisTemplatePath = genesisTemplatePath / (net.asString() + ".json");
            if (!fs::exists(configGenesisTemplatePath))
            {
                // try to load default option instead, if genesis folder exists but overrides only a few defaults
                fs::path const default_configGenesisTemplatePath = default_genesisTemplatePath / (net.asString() + ".json");
                ETH_FAIL_REQUIRE_MESSAGE(fs::exists(default_genesisTemplatePath), "default/genesis client config not found!");
                if (fs::exists(default_configGenesisTemplatePath))
                {
                    m_genesisTemplate[net] = test::readJsonData(default_configGenesisTemplatePath);

                    auto const& genesisData = m_genesisTemplate[net];
                    if (genesisData->count("params") && genesisData->atKey("params").count("chainID"))
                        m_genesisTemplateChainID[net] = spVALUE(new VALUE(genesisData->atKey("params").atKey("chainID")));

                    continue;
                }
                else
                    ETH_WARNING(string("Tried to load default config unsuccessfull: ") + default_configGenesisTemplatePath.c_str());
            }
            ETH_FAIL_REQUIRE_MESSAGE(fs::exists(configGenesisTemplatePath),
                "\ntemplate '" + net.asString() + ".json' for client '" +
                    _clientConfigPath.stem().string() + "' not found ('" +
                    configGenesisTemplatePath.c_str() + "') in configs!");
            m_genesisTemplate[net] = test::readJsonData(configGenesisTemplatePath);

            auto const& genesisData = m_genesisTemplate[net];
            if (genesisData->count("params") && genesisData->atKey("params").count("chainID"))
                m_genesisTemplateChainID[net] = spVALUE(new VALUE(genesisData->atKey("params").atKey("chainID")));
        }



        // Load correctmining Reward
        m_correctMiningRewardPath = genesisTemplatePath / "correctMiningReward.json";
        ETH_FAIL_REQUIRE_MESSAGE(fs::exists(m_correctMiningRewardPath),
            "correctMiningReward.json client config not found!");
        spDataObject correctMiningReward = test::readJsonData(m_correctMiningRewardPath);
        correctMiningReward.getContent().performModifier(mod_removeComments);
        correctMiningReward.getContent().performModifier(mod_valueToCompactEvenHexPrefixed);
        for (auto const& el : cfgFile().forks())
        {
            if (!correctMiningReward->count(el.asString()))
                ETH_FAIL_MESSAGE("Correct mining reward missing block reward record for fork: `" +
                                 el.asString() + "` (" + m_correctMiningRewardPath.string() + ")");
        }
        for (auto const& el : correctMiningReward->getSubObjects())
            m_correctReward[el->getKey()] = spVALUE(new VALUE(correctMiningReward->atKey(el->getKey())));
    }
    catch (std::exception const& _ex)
    {
        ETH_ERROR_MESSAGE(string("Error initializing configs: ") + _ex.what());
    }
}

bool ClientConfig::validateForkAllowed(FORK const& _net, bool _bail) const
{
    if (!cfgFile().allowedForks().count(_net))
    {
        ETH_WARNING("Specified network not found: '" + _net.asString() +
                    "', skipping the test. Enable the fork network in config file: " +
                    cfgFile().path().string());
        if (_bail)
            ETH_ERROR_MESSAGE("Specified network not found: '" + _net.asString() + "'");
        return false;
    }
    return true;
}

bool ClientConfig::checkForkAllowed(FORK const& _net) const
{
    return cfgFile().allowedForks().count(_net);
}

bool ClientConfig::checkForkInProgression(FORK const& _net) const
{
    return cfgFile().forkProgressionAsSet().count(_net);
}

/// translate network names in expect section field
/// >Homestead to EIP150, EIP158, Byzantium, ...
/// <=Homestead to Frontier, Homestead
void ClientConfig::translateNetworks(set<string> const& _networks, std::vector<FORK> const& _netOrder, std::vector<FORK>& _out)
{
    std::lock_guard<std::mutex> lock(g_staticDeclaration_translateNetworks_static);
    // Construct vector with test network names in a right order
    // (from Frontier to Homestead ... to Constantinople)
    // According to fork order in config file

    // Protection from putting double networks.
    // Use vector instead of set to keep the fork order
    auto addNet = [&_out](FORK const& _el) {
        for (auto const& fork : _out)
            if (fork == _el)
                return;
        _out.push_back(_el);
    };

    for (auto const& net : _networks)
    {
        std::vector<FORK> const& forkOrder = _netOrder;
        string possibleNet = net.substr(1, net.length() - 1);
        vector<FORK>::const_iterator it = std::find(forkOrder.begin(), forkOrder.end(), possibleNet);

        bool isNetworkTranslated = false;
        if (it != forkOrder.end() && net.size() > 1)
        {
            if (net[0] == '>')
            {
                while (++it != forkOrder.end())
                {
                    addNet(*it);
                    isNetworkTranslated = true;
                }
            }
            else if (net[0] == '<')
            {
                while (it != forkOrder.begin())
                {
                    addNet(*(--it));
                    isNetworkTranslated = true;
                }
            }
        }

        possibleNet = net.substr(2, net.length() - 2);
        it = std::find(forkOrder.begin(), forkOrder.end(), possibleNet);
        if (it != forkOrder.end() && net.size() > 2)
        {
            if (net[0] == '>' && net[1] == '=')
            {
                while (it != forkOrder.end())
                {
                    addNet(*(it++));
                    isNetworkTranslated = true;
                }
            }
            else if (net[0] == '<' && net[1] == '=')
            {
                _out.push_back(*it);
                while (it != forkOrder.begin())
                {
                    addNet(*(--it));
                    isNetworkTranslated = true;
                }
            }
        }

        // if nothing has been inserted, just push the untranslated network as is
        if (!isNetworkTranslated)
            addNet(net);
    }
}

std::vector<FORK> ClientConfig::translateNetworks(set<string> const& _networks) const
{
    std::vector<FORK> nets;
    ClientConfig::translateNetworks(_networks, cfgFile().forks(), nets);
    for (auto const& net : nets)
        validateForkAllowed(net);
    return nets;
}

std::string const& ClientConfig::translateException(string const& _exceptionName) const
{
    ClientConfigFile const& cfg = m_clientConfigFile;
    if (cfg.exceptions().count(_exceptionName))
        return cfg.exceptions().at(_exceptionName);

    // --- Correct typos
    // Put known exceptions into vector
    vector<string> exceptions;
    for (auto const& el : cfg.exceptions())
        exceptions.push_back(el.first);

    auto const suggestions = test::levenshteinDistance(_exceptionName, exceptions, 5);
    string message = " (Suggestions: ";
    for (auto const& el : suggestions)
        message += el + ", ";
    message += " ...)";
    ETH_ERROR_MESSAGE("Config::getExceptionString '" + _exceptionName + "' not found in client config `exceptions` section! (" +
                      cfg.path().c_str() + ")" + message);
    // ---
    static string const notfound = string();
    return notfound;
}

// Get Contents of genesis template for specified FORK
spDataObject const& ClientConfig::getGenesisTemplate(FORK const& _fork) const
{
    ETH_FAIL_REQUIRE_MESSAGE(m_genesisTemplate.count(_fork),
        "Genesis template for network '" + _fork.asString() + "' not found!");
    return m_genesisTemplate.at(_fork);
}

void ClientConfig::initializeFirstSetup()
{
    if (!m_initialized)
    {
        m_initialized = true;
        if (fs::exists(getSetupScript()))
        {
            auto cmd = [](string const& _cmd) {
                test::executeCmd(_cmd, ExecCMDWarning::NoWarning);
            };
            string const setup = getSetupScript().c_str();
            ETH_DC_MESSAGE(DC::RPC, string("Initialize setup script: ") + setup);

            thread task(cmd, setup);
            task.detach();
            size_t const initTime = cfgFile().initializeTime();
            this_thread::sleep_for(chrono::seconds(initTime));
        }
    }
}

void ClientConfig::performFieldReplace(DataObject& _data, FieldReplaceDir const& _dir) const
{
    if (cfgFile().fieldreplace().size() == 0)
        return;

    for (auto const& el : cfgFile().fieldreplace())
    {
        std::string const& retestethNotice = el.first;
        std::string const& clientNotice = el.second;

        if (_dir == FieldReplaceDir::RetestethToClient)
        {
            if (!_data.getKey().empty() && _data.getKey() == retestethNotice)
                _data.setKey(clientNotice);
        }
        else
        {
            if (!_data.getKey().empty() && _data.getKey() == clientNotice)
                _data.setKey(retestethNotice);
        }
    }

    if (_data.type() == DataType::Object || _data.type() == DataType::Array)
    {
        for (auto& obj : _data.getSubObjectsUnsafe())
            performFieldReplace(obj.getContent(), _dir);
    }
}


}  // namespace test
