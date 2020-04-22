#include <BuildInfo.h>
#include <libdevcore/FileSystem.h>
#include <retesteth/Options.h>
#include <retesteth/TestHelper.h>
#include <retesteth/configs/Genesis.h>
#include <boost/filesystem.hpp>

using namespace std;
using namespace test;
namespace fs = boost::filesystem;

namespace
{
fs::path getRetestethDataDir()
{
    fs::path const& dir = Options::get().datadir;
    bool optionsEmpty = dir.empty();
    if (!optionsEmpty && !fs::exists(dir))
        ETH_LOG(
            "Options path `" + dir.string() + "` doesn't exist, attempt to create a new directory",
            3);
    return optionsEmpty ? getDataDir("retesteth") : dir;
}
}  // namespace

void deployFirstRunConfigs()
{
    fs::path homeDir = getRetestethDataDir();

    if (fs::exists(homeDir))
    {
        if (!fs::exists(homeDir / "version"))
            return;
        string version = dev::contentsString(homeDir / "version");
        if (version != string(ETH_PROJECT_VERSION))
            ETH_WARNING("Retesteth configs version is different (running: '" +
                        string(ETH_PROJECT_VERSION) + "' vs config '" + version +
                        "')! Redeploy the configs by deleting the folder ~/.retesteth!");
        return;
    }

    // Deploy default configs
    fs::create_directory(homeDir);
    fs::create_directory(homeDir / "default");
    fs::create_directory(homeDir / "default" / "genesis");

    writeFile(homeDir / "version", string(ETH_PROJECT_VERSION));
    writeFile(homeDir / "default" / "config", default_config);
    writeFile(homeDir / "gethTCP" / "config", default_config);
    writeFile(homeDir / "besu" / "config", besu_config);
    writeFile(homeDir / "alethTCP" / "config", alethTCP_config);
    writeFile(homeDir / "alethIPCDebug" / "config", alethIPCDebug_config);
    writeFile(homeDir / "aleth" / "config", aleth_config);
    writeFile(homeDir / "aleth" / "aleth.sh", aleth_config_sh);

    fs::path genesisDir = homeDir / "default" / "genesis";

    writeFile(genesisDir / "Frontier.json", default_Frontier_config);
    writeFile(genesisDir / "Homestead.json", default_Homestead_config);
    writeFile(genesisDir / "EIP150.json", default_EIP150_config);
    writeFile(genesisDir / "EIP158.json", default_EIP158_config);
    writeFile(genesisDir / "Byzantium.json", default_Byzantium_config);
    writeFile(genesisDir / "Constantinople.json", default_Constantinople_config);
    writeFile(genesisDir / "ConstantinopleFix.json", default_ConstantinopleFix_config);
    writeFile(genesisDir / "Istanbul.json", default_Istanbul_config);

    writeFile(genesisDir / "FrontierToHomesteadAt5.json", default_FrontierToHomesteadAt5_config);
    writeFile(genesisDir / "HomesteadToDaoAt5.json", default_HomesteadToDaoAt5_config);
    writeFile(genesisDir / "HomesteadToEIP150At5.json", default_HomesteadToEIP150At5_config);
    writeFile(genesisDir / "EIP158ToByzantiumAt5.json", default_EIP158ToByzantiumAt5_config);
    writeFile(genesisDir / "ByzantiumToConstantinopleFixAt5.json",
        default_ByzantiumToConstantinopleFixAt5_config);
    writeFile(genesisDir / "correctMiningReward.json", default_correctMiningReward_config);
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
        if (cfg.getId() == _config.getId() && cfg.getName() == _config.getName())
            found = true;
    ETH_FAIL_REQUIRE_MESSAGE(
        found, "_config not found in loaded options! (DynamicOptions::setCurrentConfig)");
    m_currentConfigID = _config.getId();

    // Verify singleTestNet for the current config
    string const& net = Options::get().singleTestNet;
    if (!net.empty())
        test::checkAllowedNetwork(net, _config.getNetworks());
}

std::vector<ClientConfig> const& Options::DynamicOptions::getClientConfigs()
{
    if (m_clientConfigs.size() == 0)
    {
        deployFirstRunConfigs();

        // load the configs from options file
        std::vector<string> cfgs = Options::get().clients;
        if (cfgs.empty())
            cfgs.push_back("default");

        std::cout << "Active client configurations: '";
        for (auto const& clientName : cfgs)
            std::cout << clientName << " ";
        std::cout << "'" << std::endl;

        for (auto const& clientName : cfgs)
        {
            fs::path configDirectory = getRetestethDataDir();
            ETH_FAIL_REQUIRE_MESSAGE(fs::exists(configDirectory),
                "Could not locate provided testpath: " + string(configDirectory.c_str()));
            fs::path configPath = configDirectory / clientName;
            fs::path configFilePath = configPath / "config";
            ETH_FAIL_REQUIRE_MESSAGE(fs::exists(configFilePath),
                string("Client config not found: ") + configFilePath.c_str());

            ClientConfig cfg(configFilePath, test::readJsonData(configFilePath), ClientConfigID(),
                configPath / string(clientName + ".sh"));
            cfg.setFolderName(clientName);

            // Load genesis templates
            fs::path genesisTemplatePath = configPath / "genesis";
            if (!fs::exists(genesisTemplatePath))
            {
                genesisTemplatePath = configPath.parent_path() / "default" / "genesis";
                ETH_FAIL_REQUIRE_MESSAGE(
                    fs::exists(genesisTemplatePath), "default/genesis client config not found!");
            }

            fs::path correctMiningRewardPath = genesisTemplatePath / "correctMiningReward.json";
            ETH_FAIL_REQUIRE_MESSAGE(fs::exists(correctMiningRewardPath),
                "correctMiningReward.json client config not found!");
            cfg.setMiningRewardInfo(test::readJsonData(correctMiningRewardPath));
            cfg.setCorrectMiningRewardFilePath(correctMiningRewardPath);

            auto registerGenesisTemplate = [&cfg, &genesisTemplatePath, &clientName](
                                               string const& _net) {
                fs::path configGenesisTemplatePath = genesisTemplatePath / (_net + ".json");
                ETH_FAIL_REQUIRE_MESSAGE(fs::exists(configGenesisTemplatePath),
                    "\ntemplate '" + _net + ".json' for client '" + clientName + "' not found ('" +
                        configGenesisTemplatePath.c_str() + "') in configs!");
                cfg.addGenesisTemplate(_net, configGenesisTemplatePath);
            };
            for (auto const& net : cfg.getNetworksPlusAdditional())
                registerGenesisTemplate(net);

            //*/ Load genesis templates
            m_clientConfigs.push_back(cfg);
        }
    }
    return m_clientConfigs;
}
