#include "PrepareChainParams.h"
#include <retesteth/Options.h>
#include <retesteth/configs/ClientConfig.h>

using namespace test;
using namespace test::teststruct;

namespace  {
string calculateGenesisBaseFee(VALUE const& _currentBaseFee, ParamsContext _context)
{
    if (_context == ParamsContext::StateTests)
    {
        // Reverse back one step the baseFee calculation formula
        // to get the value for genesis block
        VALUE genesisBaseFee = _currentBaseFee * 8 / 7;
        return genesisBaseFee.asString();
    }
    else
        return _currentBaseFee.asString();
}

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
        (*genesis)["baseFeePerGas"] = calculateGenesisBaseFee(eip1559info->currentBaseFee(), _context);
        (*genesis)["difficulty"] = eip1559info->currentDifficulty().asString();
    }
    else if (_env->type() == TestEnvClass::MERGE)
    {
        StateTestEnvBaseMerge const* mergeInfo = StateTestEnvBaseMerge::castFrom(_env);
        (*genesis)["baseFeePerGas"] = calculateGenesisBaseFee(mergeInfo->currentBaseFee(), _context);
        (*genesis)["currentRandom"] = mergeInfo->currentRandom()->asString();
    }
    else
    {
        StateTestEnvBaseLegacy const* legacyInfo = StateTestEnvBaseLegacy::castFrom(_env);

        // If we are filling the test on London and it has legacy env info
        // convert this info into 1559info
        if (_net.asString() == "London")
        {
            string defaultBaseFee = "0x0a";
            // Because of the typo, blockchain tests were generated with hex "0x10" baseFee instead of dec "10"
            if (_context == ParamsContext::BlockchainTests)
                defaultBaseFee = "0x10";

            VALUE currentBaseFee((DataObject(defaultBaseFee)));
            (*genesis)["baseFeePerGas"] = calculateGenesisBaseFee(currentBaseFee, _context);
        }
        (*genesis)["difficulty"] = legacyInfo->currentDifficulty().asString();
    }

    // Convert back 1559genesis into legacy genesis
    // when filling tests with defined 1559env info
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
