#include "PrepareChainParams.h"
#include <retesteth/Options.h>
#include <retesteth/configs/ClientConfig.h>

using namespace test;
using namespace test::teststruct;

namespace test
{
namespace teststruct
{
SetChainParamsArgs prepareChainParams(FORK const& _net, SealEngine _engine, State const& _state, StateTestEnvBase const& _env)
{
    ClientConfig const& cfg = Options::get().getDynamicOptions().getCurrentConfig();
    cfg.validateForkAllowed(_net);

    DataObject genesis;
    genesis = cfg.getGenesisTemplate(_net);
    genesis["sealEngine"] = sealEngineToStr(_engine);

    genesis["genesis"]["author"] = _env.currentCoinbase().asString();
    genesis["genesis"]["difficulty"] = _env.currentDifficulty().asString();
    genesis["genesis"]["gasLimit"] = _env.currentGasLimit().asString();
    genesis["genesis"]["extraData"] = _env.currentExtraData().asString();
    genesis["genesis"]["timestamp"] = _env.currentTimestamp().asString();
    genesis["genesis"]["nonce"] = _env.currentNonce().asString();
    genesis["genesis"]["mixHash"] = _env.currentMixHash().asString();

    // Because of template might contain preset accounts
    for (auto const& el : _state.accounts())
        genesis["accounts"].addSubObject(el.second.getCContent().asDataObject());
    return SetChainParamsArgs(genesis);
}

}  // namespace teststruct
}  // namespace test
