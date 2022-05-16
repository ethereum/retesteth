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
    auto const& additional = Options::getCurrentConfig().cfgFile().additionalForks();
    bool netIsAdditional = inArray(additional, _net);

    // Convert legacy, eip1559, merge env info into one another depending on _net for execution context
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

        auto const& curConfig = Options::getCurrentConfig();
        bool knowMerge = curConfig.checkForkInProgression(FORK("Merge"));
        if (!netIsAdditional && knowMerge && compareFork(_net, CMP::ge, FORK("Merge")))
        {
            (*genesis).renameKey("difficulty", "currentRandom");
            auto const randomH32 = toCompactHexPrefixed(dev::u256((*genesis)["currentRandom"].asString()), 32);
            (*genesis)["mixHash"] = randomH32;
        }
        auto const& confPath = curConfig.getConfigPath();
        if (!knowMerge)
            ETH_WARNING(string("Client config missing required fork 'Merge': ") + confPath.c_str());
    }
    else if (_env->type() == TestEnvClass::MERGE)
    {
        StateTestEnvBaseMerge const* mergeInfo = StateTestEnvBaseMerge::castFrom(_env);
        (*genesis)["baseFeePerGas"] = calculateGenesisBaseFee(mergeInfo->currentBaseFee(), _context);
        (*genesis)["currentRandom"] = mergeInfo->currentRandom()->asString();
        auto const randomH32 = toCompactHexPrefixed(dev::u256((*genesis)["currentRandom"].asString()), 32);
        (*genesis)["mixHash"] = randomH32;
        if (!netIsAdditional && compareFork(_net, CMP::lt, FORK("Merge")))
            (*genesis)["difficulty"] = mergeInfo->currentDifficultyForOther()->asString();
    }
    else
    {
        StateTestEnvBaseLegacy const* legacyInfo = StateTestEnvBaseLegacy::castFrom(_env);
        (*genesis)["difficulty"] = legacyInfo->currentDifficulty().asString();

        // When filling old tests env section, put default values required for new networks

        auto const& curConfig = Options::getCurrentConfig();
        // If we are filling the test on London and it has legacy env info
        // convert this info into 1559info
        bool knowLondon = curConfig.checkForkInProgression(FORK("London"));
        if (!netIsAdditional && knowLondon && compareFork(_net, CMP::ge, FORK("London")))
        {
            string defaultBaseFee = "0x0a";
            // Because of the typo, blockchain tests were generated with hex "0x10" baseFee instead of dec "10"
            if (_context == ParamsContext::BlockchainTests)
                defaultBaseFee = "0x10";

            VALUE currentBaseFee((DataObject(defaultBaseFee)));
            (*genesis)["baseFeePerGas"] = calculateGenesisBaseFee(currentBaseFee, _context);
        }

        bool knowMerge = curConfig.checkForkInProgression(FORK("Merge"));
        if (!netIsAdditional && knowMerge && compareFork(_net, CMP::ge, FORK("Merge")))
        {
            (*genesis).renameKey("difficulty", "currentRandom");
            auto const randomH32 = toCompactHexPrefixed(dev::u256((*genesis)["currentRandom"].asString()), 32);
            (*genesis)["mixHash"] = randomH32;
        }

        auto const& confPath = curConfig.getConfigPath();
        if (!knowLondon)
            ETH_WARNING(string("Client config missing required fork 'London': ") + confPath.c_str());
        if (!knowMerge)
            ETH_WARNING(string("Client config missing required fork 'Merge': ") + confPath.c_str());
    }

    if (!netIsAdditional)
    {
        // Convert back 1559genesis into legacy genesis
        // when filling tests with defined 1559env info
        if ((*genesis).count("baseFeePerGas") && compareFork(_net, CMP::lt, FORK("London")))
            (*genesis).removeKey("baseFeePerGas");
        if ((*genesis).count("currentRandom") && compareFork(_net, CMP::lt, FORK("Merge")))
        {
            (*genesis).removeKey("currentRandom");
            (*genesis)["mixHash"] = _env->currentMixHash().asString();
        }
    }
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
