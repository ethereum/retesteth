#include <BuildInfo.h>
#include <libdevcore/CommonIO.h>
#include <libdevcore/FileSystem.h>
#include <retesteth/EthChecks.h>
#include <retesteth/Options.h>
#include <retesteth/configs/Options.h>
#include <retesteth/helpers/TestHelper.h>

using namespace std;
using namespace test;
using namespace test::debug;
using namespace dataobject;
using namespace dev;
using namespace test::teststruct;
namespace fs = boost::filesystem;

namespace
{
fs::path getRetestethDataDir()
{
    fs::path dataDir = Options::get().datadir;
    if (dataDir.empty())
        dataDir = dev::getDataDir("retesteth");

    if (!fs::exists(dataDir))
        ETH_DC_MESSAGE(DC::WARNING, "Options path `" + dataDir.string() + "` doesn't exist, attempt to create a new directory");
    return dataDir;
}

string prepareRetestethVersion()
{
    static const string version = string(ETH_PROJECT_VERSION) + "-" + string(ETH_VERSION_SUFFIX);
    return version;
}

}  // namespace

namespace retesteth::options
{
DataObject map_configs;
void deployFirstRunConfigs(fs::path const& _dir)
{
    // Deploy default configs
    retesteth::options::OptionsInit init;
    writeFile(_dir / "version", prepareRetestethVersion());
    for (DataObject const& cfg : map_configs.getSubObjects())
    {
        if (cfg.count("exec"))
            writeFileExec(_dir / fs::path(cfg.atKey("path").asString()), cfg.atKey("content").asString());
        else
            writeFile(_dir / fs::path(cfg.atKey("path").asString()), cfg.atKey("content").asString());
    }
}
}  // namespace retesteth::options

namespace test
{
size_t Options::DynamicOptions::activeConfigs() const
{
    return m_clientConfigs.size();
}

bool Options::DynamicOptions::currentConfigIsSet() const
{
    return m_currentConfigID.id() != ClientConfigID::null().id();
}

std::mutex g_testSuite_timeout;
void Options::DynamicOptions::setTestsuiteRunning(bool _arg)
{
    std::lock_guard<std::mutex> lock(g_testSuite_timeout);
    m_testSuiteRunning = _arg;
}
bool Options::DynamicOptions::testSuiteRunning() const
{
    std::lock_guard<std::mutex> lock(g_testSuite_timeout);
    return m_testSuiteRunning;
}

ClientConfig const& Options::DynamicOptions::getCurrentConfig() const
{
    for (auto const& cfg : m_clientConfigs)
    {
        if (cfg.getId() == m_currentConfigID)
            return cfg;
    }
    ETH_FAIL_MESSAGE("ERROR: current config not found! (DynamicOptions::getCurrentConfig())");
    return m_clientConfigs.at(0);
}

void Options::DynamicOptions::setCurrentConfig(ClientConfig const& _config)
{
    auto const& opt = Options::get();
    ETH_FAIL_REQUIRE_MESSAGE(getClientConfigs().size() > 0, "No client configs provided!");
    bool found = false;
    for (auto& cfg : m_clientConfigs)
    {
        if (cfg.getId() == _config.getId() && cfg.cfgFile().name() == _config.cfgFile().name())
        {
            found = true;
            cfg.initializeFirstSetup();
            break;
        }
    }
    ETH_FAIL_REQUIRE_MESSAGE(
        found, "_config not found in loaded options! (DynamicOptions::setCurrentConfig)");
    m_currentConfigID = _config.getId();

    // Verify singleTestNet for the current config
    string const& net = opt.singleTestNet;
    if (!net.empty())
        _config.validateForkAllowed(FORK(net));

    // Set runOnlyNetworks
    m_runOnlyNetworks.clear();
    if (!opt.runOnlyNets.empty())
    {
        auto const setOfNets = test::explodeIntoSet(opt.runOnlyNets, ',');
        auto const vectrTranslated = _config.translateNetworks(setOfNets);
        for (auto const& net : vectrTranslated)
            m_runOnlyNetworks.emplace(net);
    }
}

std::vector<ClientConfig> const& Options::DynamicOptions::getClientConfigs() const
{
    // if no Configs initialized, initialize the configs
    // Because can not initialize the configs while loading Options up
    if (m_clientConfigs.size() == 0)
    {
        fs::path const homeDir = getRetestethDataDir();
        ETH_DC_MESSAGE(DC::STATS, string("Retesteth config path: ") + homeDir.string());

        if (fs::exists(homeDir))
        {
            if (!fs::exists(homeDir / "version"))
                ETH_ERROR_MESSAGE("Missing version file in retesteth configs! `" + homeDir.string());

            string version = dev::contentsString(homeDir / "version");
            version.erase(std::remove(version.begin(), version.end(), '\n'), version.end());
            if (version != prepareRetestethVersion())
            {
                ETH_WARNING("Retesteth configs version is different (running: '" +
                             prepareRetestethVersion() + "' vs config '" + version + "')!");
                ETH_WARNING("Update configs to the latest by deleting the folder `" + homeDir.string() + "`!");
            }
        }
        else
            retesteth::options::deployFirstRunConfigs(homeDir);

        // Load the configs from options file
        std::vector<string> cfgs = Options::get().clients;
        if (cfgs.empty())
            cfgs.push_back("default");

        string clientNames;
        for (auto const& clientName : cfgs)
            clientNames += clientName + " ";
        ETH_DC_MESSAGE(DC::STATS, "Active client configurations: '" + clientNames + "'");

        for (auto const& clientName : cfgs)
            m_clientConfigs.emplace_back(ClientConfig(homeDir / clientName));
    }
    return m_clientConfigs;
}
}  // namespace test
