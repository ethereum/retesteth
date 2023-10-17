#include "PrepareChainParams.h"
#include <retesteth/Options.h>
#include <retesteth/helpers/TestHelper.h>
#include <retesteth/Constants.h>

using namespace std;
using namespace test;
using namespace test::teststruct;
using namespace test::teststruct::constnames;

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
    auto const& cfg = Options::getCurrentConfig();
    auto const& additional = cfg.cfgFile().additionalForks();
    bool netIsAdditional = inArray(additional, _net);

    // Automake plussed fork into additional nets
    if (!netIsAdditional && _net.asString().find("+") != string::npos && !cfg.cfgFile().forkProgressionAsSet().count(_net))
        netIsAdditional = true;


    // Build up RPC setChainParams genesis section
    spDataObject genesis;
    (*genesis)[c_author] = _env.currentCoinbase().asString();
    (*genesis)[c_gasLimit] = _env.currentGasLimit().asString();
    (*genesis)[c_extraData] = _env.currentExtraData().asString();
    (*genesis)[c_timestamp] = _env.currentTimestamp().asString();
    (*genesis)[c_nonce] = _env.currentNonce().asString();
    (*genesis)[c_mixHash] = _env.currentMixHash().asString();
    (*genesis)[c_difficulty] = _env.currentDifficulty().asString();

    FORK net = _net;
    if (netIsAdditional)
    {
        // Treat additional fork name `fork+` as `fork` for env section convertion
        for (auto const& fork : cfg.cfgFile().forks())
        {
            string const alteredFname = fork.asString() + "+";
            if (net.asString().find(alteredFname) != string::npos)
            {
                net = fork;
                netIsAdditional = false;
                break;
            }
        }
    }

    if (!cfg.cfgFile().support1559())
    {
        (*genesis).removeKey(c_baseFeePerGas);
        (*genesis).removeKey("currentRandom");
        (*genesis).removeKey(c_withdrawalsRoot);
        (*genesis).removeKey(c_currentExcessBlobGas);
        return genesis;
    }

    auto londify = [&_env, &_context](DataObject& _genesis){
        _genesis[c_baseFeePerGas] = calculateGenesisBaseFee(_env.currentBaseFee(), _context);
    };

    auto mergify = [&_env](DataObject& _genesis){
        _genesis.removeKey(c_difficulty);
        _genesis["currentRandom"] = _env.currentRandom().asString();
        auto const randomH32 = dev::toCompactHexPrefixed(dev::u256(_genesis["currentRandom"].asString()), 32);
        _genesis[c_mixHash] = randomH32;
    };

    auto shangfy = [&_env, &mergify](DataObject& _genesis){
        mergify(_genesis);
        _genesis[c_withdrawalsRoot] = _env.currentWithdrawalsRoot().asString();
    };

    auto cancunfy = [&_env, &shangfy](DataObject& _genesis){
        shangfy(_genesis);
        _genesis[c_currentBlobGasUsed] = _env.currentBlobGasUsed().asString();
        _genesis[c_currentExcessBlobGas] = _env.currentExcessBlobGas().asString();
        _genesis[c_currentBeaconRoot] = _env.currentBeaconRoot().asString();
    };

    if (!netIsAdditional)
    {
        bool knowLondon = cfg.checkForkInProgression("London");
        if (knowLondon && compareFork(net, CMP::ge, FORK("London")))
            londify(genesis.getContent());

        bool knowMerge = cfg.checkForkInProgression("Merge");
        if (knowMerge && compareFork(net, CMP::ge, FORK("Merge")))
            mergify(genesis.getContent());

        bool knowShaghai = cfg.checkForkInProgression("Shanghai");
        if (knowShaghai && compareFork(net, CMP::ge, FORK("Shanghai")))
            shangfy(genesis.getContent());

        bool knowCancun = cfg.checkForkInProgression("Cancun");
        if (knowCancun && compareFork(net, CMP::ge, FORK("Cancun")))
            cancunfy(genesis.getContent());
    }
    else
    {
        // Net Is Additional, probably special transition net.
        // Can't get rid of this hardcode configs :(((
        if (_net == FORK("ArrowGlacierToMergeAtDiffC0000") || _net == FORK("ArrowGlacier"))
            londify(genesis.getContent());
        else if (_net == FORK("MergeToShanghaiAtTime15k"))
        {
            londify(genesis.getContent());
            mergify(genesis.getContent());
        }
        else if (_net == FORK("ShanghaiToCancunAtTime15k"))
        {
            londify(genesis.getContent());
            mergify(genesis.getContent());
            shangfy(genesis.getContent());;
        }
    }
    return genesis;
}

void overrideChainIDByOptions(spDataObject& _genesis)
{
    if (Options::get().chainid.initialized())
    {
        if ((*_genesis).count("params"))
        {
            string const chainIDOverride = dev::toCompactHexPrefixed((size_t)Options::get().chainid);
            if ((*_genesis).atKey("params").count("chainID"))
                (*_genesis).atKeyUnsafe("params").atKeyUnsafe("chainID") = chainIDOverride;
            else
                (*_genesis).atKeyUnsafe("params")["chainID"] = chainIDOverride;
        }
    }
}

}


namespace test::teststruct
{
spSetChainParamsArgs prepareChainParams(
    FORK const& _net, SealEngine _engine, State const& _state, StateTestEnvBase const& _env, ParamsContext _context)
{
    ClientConfig const& cfg = Options::get().getDynamicOptions().getCurrentConfig();
    cfg.validateForkAllowed(_net);

    spDataObject genesis;
    (*genesis).copyFrom(cfg.getGenesisTemplate(_net).getCContent()); // TODO need copy?
    overrideChainIDByOptions(genesis);

    (*genesis)["sealEngine"] = sealEngineToStr(_engine);
    (*genesis).atKeyPointer("genesis") = prepareGenesisSubsection(_env, _context, _net);

    // Because of template might contain preset accounts
    for (auto const& el : _state.accounts())
        (*genesis)["accounts"].addSubObject(el.second->asDataObject());
    return spSetChainParamsArgs(new SetChainParamsArgs(genesis));
}

}  // namespace teststruct
