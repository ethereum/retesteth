#include "PrepareChainParams.h"
#include <retesteth/Options.h>
#include <retesteth/configs/ClientConfig.h>

using namespace test;
using namespace test::teststruct;

namespace  {

    spDataObject prepareGenesisSubsection(StateTestEnvBase const* _env, ParamsContext _context, FORK const& _net)
    {
        spDataObject genesis;

        (*genesis)["author"] = _env->currentCoinbase().asString();
        (*genesis)["gasLimit"] = _env->currentGasLimit().asString();
        (*genesis)["extraData"] = _env->currentExtraData().asString();
        (*genesis)["timestamp"] = _env->currentTimestamp().asString();
        (*genesis)["nonce"] = _env->currentNonce().asString();
        (*genesis)["mixHash"] = _env->currentMixHash().asString();

        if (_env->type() == TestEnvClass::EIP1559)
        {
            StateTestEnvBase1559 const* eip1559info = StateTestEnvBase1559::castFrom(_env);
            if (_context == ParamsContext::StateTests)
            {
                // Reverse back the baseFee calculation formula for genesis block
                VALUE const& baseFee = eip1559info->currentBaseFee().getCContent();
                VALUE genesisBaseFee = baseFee * 8 / 7;
                (*genesis)["baseFeePerGas"] = genesisBaseFee.asString();
            }
            else
                (*genesis)["baseFeePerGas"] = eip1559info->currentBaseFee()->asString();
            (*genesis)["difficulty"] = eip1559info->currentDifficulty().asString();
        }
        else
        {
            StateTestEnvBaseLegacy const* legacyInfo = StateTestEnvBaseLegacy::castFrom(_env);
            // set baseFee for legacy headers running on 1559 upgrade
            if (_net.asString() == "London")
            {
                if (_context == ParamsContext::StateTests)
                {
                    VALUE genesisBaseFee = 10 * 8 / 7;
                    (*genesis)["baseFeePerGas"] = genesisBaseFee.asString();
                }
                else
                    (*genesis)["baseFeePerGas"] = "0x10";
            }
            (*genesis)["difficulty"] = legacyInfo->currentDifficulty().asString();
        }

        // Convert back 1559 genesis into legacy genesis, when filling 1559 tests
        auto const& additional = Options::getCurrentConfig().cfgFile().additionalForks();
        if (!inArray(additional, _net) && compareFork(_net, CMP::lt, FORK("London")))
            (*genesis).removeKey("baseFeePerGas");

        return genesis;
    }
}


namespace test
{
namespace teststruct
{
spSetChainParamsArgs prepareChainParams(
    FORK const& _net, SealEngine _engine, State const& _state, StateTestEnvBase const* _env, ParamsContext _context)
{
    ClientConfig const& cfg = Options::get().getDynamicOptions().getCurrentConfig();
    cfg.validateForkAllowed(_net);

    spDataObject genesis;
    (*genesis).copyFrom(cfg.getGenesisTemplate(_net).getCContent()); // TODO need copy?
    (*genesis)["sealEngine"] = sealEngineToStr(_engine);
    (*genesis).atKeyPointer("genesis") = prepareGenesisSubsection(_env, _context, _net);

    // Because of template might contain preset accounts
    for (auto const& el : _state.accounts())
        (*genesis)["accounts"].addSubObject(el.second->asDataObject());
    return spSetChainParamsArgs(new SetChainParamsArgs(genesis));
}

}  // namespace teststruct
}  // namespace test
