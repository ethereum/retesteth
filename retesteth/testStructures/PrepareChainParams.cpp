#include "PrepareChainParams.h"
#include <retesteth/Options.h>
#include <retesteth/TestHelper.h>
#include <retesteth/configs/ClientConfig.h>

using namespace std;
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

spDataObject prepareGenesisSubsection(StateTestEnvBase const& _env, ParamsContext _context, FORK const& _net)
{
    auto const& additional = Options::getCurrentConfig().cfgFile().additionalForks();
    bool netIsAdditional = inArray(additional, _net);

    // Build up RPC setChainParams genesis section
    spDataObject genesis;
    (*genesis)["author"] = _env.currentCoinbase().asString();
    (*genesis)["gasLimit"] = _env.currentGasLimit().asString();
    (*genesis)["extraData"] = _env.currentExtraData().asString();
    (*genesis)["timestamp"] = _env.currentTimestamp().asString();
    (*genesis)["nonce"] = _env.currentNonce().asString();
    (*genesis)["mixHash"] = _env.currentMixHash().asString();
    (*genesis)["difficulty"] = _env.currentDifficulty().asString();

    if (!netIsAdditional)
    {
        auto const& cfg = Options::getCurrentConfig();

        bool knowLondon = cfg.checkForkInProgression("London");
        if (knowLondon && compareFork(_net, CMP::ge, FORK("London")))
            (*genesis)["baseFeePerGas"] = calculateGenesisBaseFee(_env.currentBaseFee(), _context);

        bool knowMerge = cfg.checkForkInProgression("Merge");
        if (knowMerge && compareFork(_net, CMP::ge, FORK("Merge")))
        {
            (*genesis).removeKey("difficulty");
            (*genesis)["currentRandom"] = _env.currentRandom().asString();
            auto const randomH32 = dev::toCompactHexPrefixed(dev::u256((*genesis)["currentRandom"].asString()), 32);
            (*genesis)["mixHash"] = randomH32;
        }
    }
    else
    {
        // Net Is Additional, probably special transition net.
        // Can't get rid of this hardcode configs :(((
        if (_net == FORK("ArrowGlacierToMergeAtDiffC0000") || _net == FORK("ArrowGlacier"))
            (*genesis)["baseFeePerGas"] = calculateGenesisBaseFee(_env.currentBaseFee(), _context);

    }
    return genesis;
}}


namespace test
{
namespace teststruct
{
spSetChainParamsArgs prepareChainParams(
    FORK const& _net, SealEngine _engine, State const& _state, StateTestEnvBase const& _env, ParamsContext _context)
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
