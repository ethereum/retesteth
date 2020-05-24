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
/*void checkDataObject(DataObject const& _input)
{
    ETH_ERROR_REQUIRE_MESSAGE(_input.type() == DataType::Object,
        TestOutputHelper::get().get().testFile().string() +
            " A test file must contain an object value (json/yaml).");
}*/

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

DataObject prepareChainParams(FORK const& _net, SealEngine _engine, State const& _state, StateTestEnvBase const& _env)
{
    ClientConfig const& cfg = Options::get().getDynamicOptions().getCurrentConfig();
    cfg.validateForkAllowed(_net);

    DataObject genesis;
    genesis = cfg.getGenesisTemplate(_net);
    genesis["sealEngine"] = sealEngineToStr(_engine);

    genesis["genesis"]["author"] = _env.currentCoinbase().asString();
    genesis["genesis"]["difficulty"] = _env.currentDifficulty().asString();
    genesis["genesis"]["gasLimit"] = _env.currentGasLimit().asString();
    genesis["genesis"]["extraData"] = "0x00";
    genesis["genesis"]["timestamp"] = "0x00";  //_env.currentTimestamp().asString();
    genesis["genesis"]["nonce"] = "0x0000000000000000";
    genesis["genesis"]["mixHash"] = "0x0000000000000000000000000000000000000000000000000000000000000000";

    // Because of template might contain preset accounts
    for (auto const& el : _state.accounts())
        genesis["accounts"].addSubObject(el.second.getCContent().asDataObject());
    return genesis;

    // CHAIN PARAMS AS A STRUCTURE ???
}


}  // namespace teststruct
}  // namespace test
