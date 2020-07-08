#include <BuildInfo.h>
#include <libdevcore/CommonIO.h>
#include <libdevcore/FileSystem.h>
#include <retesteth/Options.h>
#include <retesteth/configs/Genesis.h>
#include <boost/filesystem.hpp>

using namespace std;
using namespace test;
namespace fs = boost::filesystem;

namespace
{
fs::path getRetestethDataDir()
{
    fs::path dataDir = Options::get().datadir;
    if (dataDir.empty())
        dataDir = getDataDir("retesteth");

    if (!fs::exists(dataDir))
        ETH_LOG("Options path `" + dataDir.string() + "` doesn't exist, attempt to create a new directory", 3);
    return dataDir;
}
}  // namespace

string prepareRetestethVersion()
{
    static const string version = string(ETH_PROJECT_VERSION) + "-" + string(ETH_VERSION_SUFFIX);
    return version;
}

void deployFirstRunConfigs(fs::path const& _dir)
{
    // Deploy default configs
    fs::create_directory(_dir);
    fs::create_directory(_dir / "default");
    fs::create_directory(_dir / "default" / "genesis");

    writeFile(_dir / "version", prepareRetestethVersion());
    writeFile(_dir / "default" / "config", default_config);
    writeFile(_dir / "t8ntool" / "config", t8ntool_config);
    writeFile(_dir / "gethTCP" / "config", default_config);
    writeFile(_dir / "besu" / "config", besu_config);
    writeFile(_dir / "alethTCP" / "config", alethTCP_config);
    writeFile(_dir / "alethIPCDebug" / "config", alethIPCDebug_config);
    writeFile(_dir / "aleth" / "config", aleth_config);
    writeFile(_dir / "aleth" / "aleth.sh", aleth_config_sh);

    // Default geth configs
    fs::path genesisDir = _dir / "default" / "genesis";
    writeFile(genesisDir / "Frontier.json", default_Frontier_config);
    writeFile(genesisDir / "Homestead.json", default_Homestead_config);
    writeFile(genesisDir / "EIP150.json", default_EIP150_config);
    writeFile(genesisDir / "EIP158.json", default_EIP158_config);
    writeFile(genesisDir / "Byzantium.json", default_Byzantium_config);
    writeFile(genesisDir / "Constantinople.json", default_Constantinople_config);
    writeFile(genesisDir / "ConstantinopleFix.json", default_ConstantinopleFix_config);
    writeFile(genesisDir / "Istanbul.json", default_Istanbul_config);
    writeFile(genesisDir / "Berlin.json", default_Berlin_config);

    writeFile(genesisDir / "FrontierToHomesteadAt5.json", default_FrontierToHomesteadAt5_config);
    writeFile(genesisDir / "HomesteadToDaoAt5.json", default_HomesteadToDaoAt5_config);
    writeFile(genesisDir / "HomesteadToEIP150At5.json", default_HomesteadToEIP150At5_config);
    writeFile(genesisDir / "EIP158ToByzantiumAt5.json", default_EIP158ToByzantiumAt5_config);
    writeFile(genesisDir / "ByzantiumToConstantinopleFixAt5.json",
        default_ByzantiumToConstantinopleFixAt5_config);
    writeFile(genesisDir / "correctMiningReward.json", default_correctMiningReward_config);

    // Default geth t8ntool configs
    genesisDir = _dir / "t8ntool" / "genesis";
    writeFile(genesisDir / "Frontier.json", t8ntool_Frontier_config);
    writeFile(genesisDir / "Homestead.json", t8ntool_Homestead_config);
    writeFile(genesisDir / "EIP150.json", t8ntool_EIP150_config);
    writeFile(genesisDir / "EIP158.json", t8ntool_EIP158_config);
    writeFile(genesisDir / "Byzantium.json", t8ntool_Byzantium_config);
    writeFile(genesisDir / "Constantinople.json", t8ntool_Constantinople_config);
    writeFile(genesisDir / "ConstantinopleFix.json", t8ntool_ConstantinopleFix_config);
    writeFile(genesisDir / "Istanbul.json", t8ntool_Istanbul_config);
    writeFile(genesisDir / "Berlin.json", t8ntool_Berlin_config);

    writeFile(genesisDir / "FrontierToHomesteadAt5.json", t8ntool_FrontierToHomesteadAt5_config);
    writeFile(genesisDir / "HomesteadToDaoAt5.json", t8ntool_HomesteadToDaoAt5_config);
    writeFile(genesisDir / "HomesteadToEIP150At5.json", t8ntool_HomesteadToEIP150At5_config);
    writeFile(genesisDir / "EIP158ToByzantiumAt5.json", t8ntool_EIP158ToByzantiumAt5_config);
    writeFile(genesisDir / "ByzantiumToConstantinopleFixAt5.json",
        t8ntool_ByzantiumToConstantinopleFixAt5_config);
    writeFile(genesisDir / "correctMiningReward.json", t8ntool_correctMiningReward_config);
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
    ETH_FAIL_REQUIRE_MESSAGE(getClientConfigs().size() > 0, "No client configs provided!");
    bool found = false;
    for (auto const& cfg : getClientConfigs())
        if (cfg.getId() == _config.getId() && cfg.cfgFile().name() == _config.cfgFile().name())
            found = true;
    ETH_FAIL_REQUIRE_MESSAGE(
        found, "_config not found in loaded options! (DynamicOptions::setCurrentConfig)");
    m_currentConfigID = _config.getId();

    // Verify singleTestNet for the current config
    string const& net = Options::get().singleTestNet;
    if (!net.empty())
        _config.validateForkAllowed(FORK(net));
}

std::vector<ClientConfig> const& Options::DynamicOptions::getClientConfigs()
{
    // if no Configs initialized, initialize the configs
    // Because can not initialize the configs while loading Options up
    if (m_clientConfigs.size() == 0)
    {
        fs::path homeDir = getRetestethDataDir();
        if (fs::exists(homeDir))
        {
            if (!fs::exists(homeDir / "version"))
                ETH_ERROR_MESSAGE("Missing version file in retesteth configs! " + homeDir.string());

            string version = dev::contentsString(homeDir / "version");
            if (version != prepareRetestethVersion())
                ETH_WARNING("Retesteth configs version is different (running: '" +
                            prepareRetestethVersion() + "' vs config '" + version +
                            "')! Redeploy the configs by deleting the folder ~/.retesteth!");
        }
        else
            deployFirstRunConfigs(homeDir);

        // Load the configs from options file
        std::vector<string> cfgs = Options::get().clients;
        if (cfgs.empty())
            cfgs.push_back("default");

        std::cout << "Active client configurations: '";
        for (auto const& clientName : cfgs)
            std::cout << clientName << " ";
        std::cout << "'" << std::endl;

        for (auto const& clientName : cfgs)
            m_clientConfigs.push_back(ClientConfig(homeDir / clientName));
    }
    return m_clientConfigs;
}
