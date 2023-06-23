#include <retesteth/EthChecks.h>
#include <retesteth/helpers/TestHelper.h>
#include <retesteth/helpers/TestOutputHelper.h>
#include <retesteth/configs/ClientConfig.h>
#include <retesteth/testStructures/Common.h>
#include <retesteth/Constants.h>
#include <retesteth/Options.h>
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

        m_pyspecsStartPath = _clientConfigPath.parent_path() / "pyspecsStart.sh";

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
            ETH_FAIL_REQUIRE_MESSAGE(fs::exists(genesisTemplatePath), genesisTemplatePath.string() + " client config path not found!");
        }

        // Load genesis templates
        for (auto const& net : cfgFile().allowedForks())
        {
            if (checkForkSkipOnFiller(net))
                continue;
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

        CJOptions opt { .jsonParse = CJOptions::JsonParse::ALLOW_COMMENTS };
        spDataObject correctMiningReward = test::readJsonData(m_correctMiningRewardPath, opt);
        correctMiningReward.getContent().performModifier(mod_removeComments);
        correctMiningReward.getContent().performModifier(mod_valueToCompactEvenHexPrefixed);
        for (auto const& el : cfgFile().forks())
        {
            if (!correctMiningReward->count(el.asString()))
            {
                (*correctMiningReward)[el.asString()] = "0x00";
                ETH_DC_MESSAGE(DC::STATS2, "Correct mining reward init default reward '0' for fork: `" +
                            el.asString() + "` (" + m_correctMiningRewardPath.string() + ")");
            }
        }
        for (auto const& el : correctMiningReward->getSubObjects())
            m_correctReward[el->getKey()] = sVALUE(correctMiningReward->atKey(el->getKey()));
    }
    catch (std::exception const& _ex)
    {
        ETH_ERROR_MESSAGE(string("Error initializing configs: ") + _ex.what());
    }
}


bool tryLookPlussedFork(ClientConfigFile const& _cfgFile, FORK const& _net)
{
    string const plussedFork = makePlussedFork(_net);
    if (!plussedFork.empty())
        return _cfgFile.allowedForks().count(plussedFork);
    return false;
}

bool ClientConfig::validateForkAllowed(FORK const& _net, bool _bail) const
{
    bool checkForkDefenition = cfgFile().allowedForks().count(_net);
    if (!checkForkDefenition)
        checkForkDefenition = tryLookPlussedFork(cfgFile(), _net);

    if (!checkForkDefenition)
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
    bool checkForkDefenition = cfgFile().allowedForks().count(_net);
    if (!checkForkDefenition)
        checkForkDefenition = tryLookPlussedFork(cfgFile(), _net);
    return checkForkDefenition;
}

bool ClientConfig::checkForkInProgression(FORK const& _net) const
{
    return cfgFile().forkProgressionAsSet().count(_net);
}

bool ClientConfig::checkForkSkipOnFiller(FORK const& _net) const
{
    for (auto const& fork : cfgFile().fillerSkipForks())
    {
        if (fork == _net)
            return true;
    }
    return false;
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
        if (net.size() < 1)
            ETH_ERROR_MESSAGE("ClientConfig::translateNetworks:: Given net is too short `" + net + "`");
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
    string const error = "Config::getExceptionString '" + _exceptionName + "' not found in client config `exceptions` section! (" +
                         cfg.path().c_str() + ")" + message;
    ETH_DC_MESSAGE(DC::STATS2, error);
    return _exceptionName;

    // ---
    return C_EMPTY_STR;
}

// Get Contents of genesis template for specified FORK
spDataObject ClientConfig::getGenesisTemplate(FORK const& _fork) const
{
    bool const templateHasFork = m_genesisTemplate.count(_fork);
    if (!templateHasFork)
    {
        string const plussedFork = makePlussedFork(_fork);
        if (!plussedFork.empty() && m_genesisTemplate.count(plussedFork))
        {
            auto const& origTemplate = m_genesisTemplate.at(plussedFork).getCContent();
            if (origTemplate.count("params") && origTemplate.atKey("params").count("fork"))
            {
                spDataObject tmplateOriginal = m_genesisTemplate.at(plussedFork)->copy();
                size_t pos = _fork.asString().find("+");
                tmplateOriginal.getContent().atKeyUnsafe("params").atKeyUnsafe("fork").asStringUnsafe() +=
                    _fork.asString().substr(pos);
                return tmplateOriginal;
            }
        }
    }

    ETH_FAIL_REQUIRE_MESSAGE(templateHasFork, "Genesis template for network '" + _fork.asString() + "' not found!");
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
                int exitCode;
                test::executeCmd(_cmd, exitCode, ExecCMDWarning::NoWarning);
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

spVALUE const& ClientConfig::getRewardForFork(FORK const& _fork) const
{
    // Load rewards for 'fork' from 'fork+xxxx'
    FORK const& fork = _fork;
    bool hasReward = m_correctReward.count(_fork);
    if (!hasReward)
    {
        string const forkPlussed = test::makePlussedFork(_fork);
        if (!forkPlussed.empty())
        {
            hasReward = m_correctReward.count(forkPlussed);
            if (hasReward)
                return m_correctReward.at(forkPlussed);
        }
    }
    ETH_ERROR_REQUIRE_MESSAGE(m_correctReward.count(fork),
        "Network '" + fork.asString() + "' not found in correct mining info config (" +
            getRewardMapPath().string() + ") Client: " + cfgFile().name());
    return m_correctReward.at(fork);
}


}  // namespace test
