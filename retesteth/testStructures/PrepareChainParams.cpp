#include "PrepareChainParams.h"
#include <retesteth/Options.h>
#include <retesteth/configs/ClientConfig.h>

using namespace test;
using namespace test::teststruct;

namespace test
{
namespace teststruct
{
SetChainParamsArgs prepareChainParams(
    FORK const& _net, SealEngine _engine, State const& _state, StateTestEnvBase const& _env, ParamsContext _paramsContext)
{
    ClientConfig const& cfg = Options::get().getDynamicOptions().getCurrentConfig();
    cfg.validateForkAllowed(_net);

    DataObject genesis;
    genesis = cfg.getGenesisTemplate(_net);
    genesis["sealEngine"] = sealEngineToStr(_engine);

    genesis["genesis"]["author"] = _env.currentCoinbase().asString();
    genesis["genesis"]["difficulty"] = _env.currentDifficulty().asString();
    genesis["genesis"]["gasLimit"] = _env.currentGasLimit().asString();

    if (!_env.currentBaseFee().isEmpty())
    {
        if (_paramsContext == ParamsContext::StateTests)
        {
            // Reverse back the baseFee calculation formula for genesis block
            VALUE const& baseFee = _env.currentBaseFee().getCContent();
            VALUE genesisBaseFee = baseFee * 8 / 7;
            genesis["genesis"]["baseFee"] = genesisBaseFee.asString();
        }
        else
            genesis["genesis"]["baseFee"] = _env.currentBaseFee()->asString();
    }
    else
    {
        // set baseFee for legacy headers running on 1559 upgrade
        if (_net.asString() == "London")
        {
            if (_paramsContext == ParamsContext::StateTests)
            {
                VALUE genesisBaseFee = 10 * 8 / 7;
                genesis["genesis"]["baseFee"] = genesisBaseFee.asString();
            }
            else
                genesis["genesis"]["baseFee"] = "0x10";
        }
    }

    // Convert back 1559 genesis into legacy genesis, when filling 1559 tests
    auto const& additional = Options::getCurrentConfig().cfgFile().additionalForks();
    if (!inArray(additional, _net) && compareFork(_net, CMP::lt, FORK("London")))
        genesis.atKeyUnsafe("genesis").removeKey("baseFee");

    genesis["genesis"]["extraData"] = _env.currentExtraData().asString();
    genesis["genesis"]["timestamp"] = _env.currentTimestamp().asString();
    genesis["genesis"]["nonce"] = _env.currentNonce().asString();
    genesis["genesis"]["mixHash"] = _env.currentMixHash().asString();

    // Because of template might contain preset accounts
    for (auto const& el : _state.accounts())
        genesis["accounts"].addSubObject(el.second->asDataObject());
    return SetChainParamsArgs(genesis);
}

}  // namespace teststruct
}  // namespace test
