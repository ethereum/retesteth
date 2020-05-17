#include "Common.h"
#include "EthChecks.h"
#include "TestHelper.h"
#include "TestOutputHelper.h"
#include <retesteth/Options.h>
#include <retesteth/configs/ClientConfig.h>
#include <retesteth/ethObjects/object.h>

using namespace test;
using namespace test::teststruct;

namespace test
{
namespace teststruct
{
void checkDataObject(DataObject const& _input)
{
    ETH_ERROR_REQUIRE_MESSAGE(_input.type() == DataType::Object,
        TestOutputHelper::get().get().testFile().string() +
            " A test file must contain an object value (json/yaml).");
}

/*
void checkAllowedNetwork(string const& _network, vector<string> const& _networkOrder)
{
    if (!std::find(_networkOrder.begin(), _networkOrder.end(), _network))
    {
        fs::path const& configFile =
Options::getDynamicOptions().getCurrentConfig().getConfigFilePath(); ETH_WARNING( "Specified network
not found: '" + _network +
            "', skipping the test. Enable the fork network in config file: " + configFile.string());
        ETH_ERROR_MESSAGE("Specified network not found: '" + _network + "'");
    }
}*/

string prepareChainParams()
{
    /*
    ClientConfig const& cfg = Options::get().getDynamicOptions().getCurrentConfig();
    test::checkAllowedNetwork(_network, cfg.getNetworksPlusAdditional());

    DataObject genesis;
    genesis = cfg.getGenesisTemplate(_network);
    genesis["sealEngine"] = _engine;
    return genesis;

    genesis["genesis"] = getEnv().getDataForRPC();
    for (auto const& acc : getPre().getData().getSubObjects())
        genesis["accounts"].addSubObject(acc);
    return genesis;*/
    return "";
}


}  // namespace teststruct
}  // namespace test
