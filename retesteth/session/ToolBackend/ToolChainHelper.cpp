#include "Verification.h"
#include <retesteth/helpers/TestHelper.h>
#include <retesteth/Options.h>
#include <retesteth/testStructures/Common.h>
#include <retesteth/Constants.h>
using namespace dev;
using namespace test;
using namespace std;
using namespace test::debug;
using namespace teststruct;
using namespace dataobject;
using namespace test::teststruct::constnames;
namespace fs = boost::filesystem;

namespace  {
FORK convertForkToToolConfig(FORK const& _fork)
{
    auto const& genesisSetupInTool = Options::getCurrentConfig().getGenesisTemplate(_fork);
    FORK const t8nForkName(genesisSetupInTool.getCContent().atKey("params").atKey("fork").asString());
    return t8nForkName;
}
}

namespace toolimpl
{
ToolParams::ToolParams(DataObject const& _data)
{
    REQUIRE_JSONFIELDS(_data, "ToolParams " + _data.getKey(),
        {{"fork", {{DataType::String}, jsonField::Required}},
            {"muirGlacierForkBlock", {{DataType::String}, jsonField::Optional}},
            {"constantinopleForkBlock", {{DataType::String}, jsonField::Optional}},
            {"byzantiumForkBlock", {{DataType::String}, jsonField::Optional}},
            {"londonForkBlock", {{DataType::String}, jsonField::Optional}},
            {"homesteadForkBlock", {{DataType::String}, jsonField::Optional}},
            {"terminalTotalDifficulty", {{DataType::String}, jsonField::Optional}},
            {"chainID", {{DataType::String}, jsonField::Optional}}
        });

    const bigint unreachable = 10000000000;
    if (_data.count("homesteadForkBlock"))
        m_homesteadForkBlock = sVALUE(_data.atKey("homesteadForkBlock"));
    else
        m_homesteadForkBlock = sVALUE(unreachable);

    if (_data.count("byzantiumForkBlock"))
        m_byzantiumForkBlock = sVALUE(_data.atKey("byzantiumForkBlock"));
    else
        m_byzantiumForkBlock = sVALUE(unreachable);

    if (_data.count("constantinopleForkBlock"))
        m_constantinopleForkBlock = sVALUE(_data.atKey("constantinopleForkBlock"));
    else
        m_constantinopleForkBlock = sVALUE(unreachable);

    if (_data.count("muirGlacierForkBlock"))
        m_muirGlacierForkBlock = sVALUE(_data.atKey("muirGlacierForkBlock"));
    else
        m_muirGlacierForkBlock = sVALUE(unreachable);

    if (_data.count("londonForkBlock"))
        m_londonForkBlock = sVALUE(_data.atKey("londonForkBlock"));
    else
        m_londonForkBlock = sVALUE(unreachable);
}

// We simulate the client backend side here, so thats why number5 is hardcoded
// Map rewards from non standard forks into standard
static std::map<FORK, FORK> RewardMapForToolBefore5 = {
    {"FrontierToHomesteadAt5", "Frontier"},
    {"HomesteadToEIP150At5", "Homestead"},
    {"EIP158ToByzantiumAt5", "EIP158"},
    {"HomesteadToDaoAt5", "Homestead"},
    {"ByzantiumToConstantinopleFixAt5", "Byzantium"},
    {"BerlinToLondonAt5", "Berlin"},
    {"ArrowGlacierToMergeAtDiffC0000", "ArrowGlacier"},
    {"MergeToShanghaiAtTime15k", "Merge"},
    {"ShanghaiToCancunAtTime15k", "Shanghai"}
};
static std::map<FORK, FORK> RewardMapForToolAfter5 = {
    {"FrontierToHomesteadAt5", "Homestead"},
    {"HomesteadToEIP150At5", "EIP150"},
    {"EIP158ToByzantiumAt5", "Byzantium"},
    {"HomesteadToDaoAt5", "Homestead"},
    {"ByzantiumToConstantinopleFixAt5", "ConstantinopleFix"},
    {"BerlinToLondonAt5", "London"},
    {"ArrowGlacierToMergeAtDiffC0000", "Merge"},
    {"MergeToShanghaiAtTime15k", "Shanghai"},
    {"ShanghaiToCancunAtTime15k", "Cancun"}
};

std::tuple<VALUE, FORK> prepareReward(SealEngine _engine, FORK const& _fork, EthereumBlockState const& _curBlockRef)
{
    if (_engine == SealEngine::Ethash)
        ETH_DC_MESSAGE(DC::LOWLOG, "t8ntool backend treat Ethash as NoProof!");

    bool isMerge = false;
    bool posTransitionDifficultyNotReached = false;
    bool timestampTransitionNotReached = false;
    if (_fork.asString() == "ArrowGlacierToMergeAtDiffC0000")
    {
        isMerge = true;
        // The TD here is the one before tool called for mining. so its like n-1 td.
        if (_curBlockRef.totalDifficulty() < VALUE(DataObject("0x0C0000")))
            posTransitionDifficultyNotReached = true;
    }
    else if (_fork.asString() == "MergeToShanghaiAtTime15k"
             || _fork.asString() == "ShanghaiToCancunAtTime15k")
    {
        isMerge = true;
        if (_curBlockRef.header()->timestamp() < 15000)
            timestampTransitionNotReached = true;
    }

    // Setup mining rewards
    std::map<FORK, spVALUE> const& rewards = Options::get().getCurrentConfig().getRewardMap();

    // Load rewards for 'fork' from 'fork+xxxx'
    FORK const& fork = _fork;
    bool hasReward = rewards.count(_fork);
    if (!hasReward)
    {
        string const forkPlussed = makePlussedFork(_fork);
        if (!forkPlussed.empty())
        {
            hasReward = rewards.count(forkPlussed);
            if (hasReward)
                return {rewards.at(forkPlussed).getCContent(), _fork};
        }
    }

    if (hasReward)
        return {rewards.at(fork).getCContent(), _fork};
    else
    {
        if ((!isMerge && _curBlockRef.header()->number() < 5)
            || posTransitionDifficultyNotReached
            || timestampTransitionNotReached)
        {
            if (!RewardMapForToolBefore5.count(fork))
            {
                fs::path const& rewardMapPath = Options::get().getDynamicOptions().getCurrentConfig().getRewardMapPath();
                ETH_ERROR_MESSAGE("ToolBackend error getting reward for fork: " + fork.asString() +
                                  ", check that fork reward is defined at (" + rewardMapPath.c_str() + ")");
            }
            auto const& trFork = convertForkToToolConfig(RewardMapForToolBefore5.at(fork));
            assert(rewards.count(trFork));
            return {rewards.at(trFork).getCContent(), trFork};
        }
        else
        {
            if (!RewardMapForToolAfter5.count(fork))
            {
                fs::path const& rewardMapPath = Options::get().getDynamicOptions().getCurrentConfig().getRewardMapPath();
                ETH_ERROR_MESSAGE("ToolBackend error getting reward for fork: " + fork.asString() +
                                  ", check that fork reward is defined at " + rewardMapPath.c_str() + ")");
            }
            auto const& trFork = convertForkToToolConfig(RewardMapForToolAfter5.at(fork));
            assert(rewards.count(trFork));
            return {rewards.at(trFork).getCContent(), fork == "HomesteadToDaoAt5" ? "HomesteadToDaoAt5" : trFork};
        }
    }
}

VALUE calculateGasLimit(VALUE const& _parentGasLimit, VALUE const& _parentGasUsed)
{
    //_gasFloorTarget == Invalid256 ? 3141562 : _gasFloorTarget;
    static const bigint gasFloorTarget = 3141562;
    const bigint gasLimit = _parentGasLimit.asBigInt();
    static const bigint boundDivisor = bigint("0x0400");
    if (gasLimit < gasFloorTarget)
        return min<bigint>(gasFloorTarget, gasLimit + gasLimit / boundDivisor - 1);
    else
        return max<bigint>(
            gasFloorTarget, gasLimit - gasLimit / boundDivisor + 1 + (_parentGasUsed.asBigInt() * 6 / 5) / boundDivisor);
}

// Because tool report incomplete state. restore missing fields with zeros
// Also remove leading zeros in storage
spState restoreFullState(DataObject& _toolState)
{
    spDataObject fullState;
    for (auto& accTool2 : _toolState.getSubObjectsUnsafe())
    {
        DataObject& accTool = accTool2.getContent();
        DataObject& acc = fullState.getContent()[accTool.getKey()];
        acc[c_balance] = accTool.count(c_balance) ? accTool.atKey(c_balance).asString() : "0x00";
        acc[c_nonce] = accTool.count(c_nonce) ? accTool.atKey(c_nonce).asString() : "0x00";
        acc[c_code] = accTool.count(c_code) ? accTool.atKey(c_code).asString() : "0x";
        if (accTool.count(c_storage))
            acc.atKeyPointer(c_storage) = accTool.atKeyPointerUnsafe(c_storage);
        else
            acc.atKeyPointer(c_storage) = sDataObject(DataType::Object);
        for (auto& storageRecord : acc.atKeyUnsafe(c_storage).getSubObjectsUnsafe())
        {
            storageRecord.getContent().performModifier(mod_removeLeadingZerosFromHexValueEVEN);
            storageRecord.getContent().performModifier(mod_removeLeadingZerosFromHexKeyEVEN);
        }
    }
    return spState(new State(dataobject::move(fullState)));
}

ChainOperationParams ChainOperationParams::defaultParams(ToolParams const& _params)
{
    ChainOperationParams aleth;
    aleth.durationLimit = u256("0x0d");
    aleth.minimumDifficulty = u256("0x20000");
    aleth.difficultyBoundDivisor = u256("0x0800");
    aleth.homesteadForkBlock = _params.homesteadForkBlock().asBigInt();
    aleth.byzantiumForkBlock = _params.byzantiumForkBlock().asBigInt();
    aleth.constantinopleForkBlock = _params.constantinopleForkBlock().asBigInt();
    aleth.muirGlacierForkBlock = _params.muirGlacierForkBlock().asBigInt();
    aleth.londonForkBlock = _params.londonForkBlock().asBigInt();
    return aleth;
}

// Aleth calculate difficulty formula
VALUE calculateEthashDifficulty(
    ChainOperationParams const& _chainParams, BlockHeader const& _bi, BlockHeader const& _parent)
{
    const unsigned c_expDiffPeriod = 100000;

    if (_bi.number() == 0)
        throw test::UpwardsException("calculateEthashDifficulty was called for block with number == 0");

    auto const& minimumDifficulty = _chainParams.minimumDifficulty;
    auto const& difficultyBoundDivisor = _chainParams.difficultyBoundDivisor;
    auto const& durationLimit = _chainParams.durationLimit;

    VALUE target(0);  // stick to a bigint for the target. Don't want to risk going negative.
    if (_bi.number() < _chainParams.homesteadForkBlock)
    {
        // Frontier-era difficulty adjustment
        target = _bi.timestamp() >= _parent.timestamp() + durationLimit ?
                     _parent.difficulty() - (_parent.difficulty() / difficultyBoundDivisor) :
                     (_parent.difficulty() + (_parent.difficulty() / difficultyBoundDivisor));
    }
    else
    {
        VALUE const timestampDiff = _bi.timestamp() - _parent.timestamp();
        VALUE const adjFactor =
            _bi.number() < _chainParams.byzantiumForkBlock ?
                max<bigint>(1 - timestampDiff.asBigInt() / 10, -99) :  // Homestead-era difficulty adjustment
                max<bigint>((_parent.hasUncles() ? 2 : 1) - timestampDiff.asBigInt() / 9,
                    -99);  // Byzantium-era difficulty adjustment

        target = _parent.difficulty() + _parent.difficulty() / 2048 * adjFactor;
    }

    VALUE o = target;
    unsigned exponentialIceAgeBlockNumber = (unsigned)_parent.number().asBigInt() + 1;

    // EIP-2384 Istanbul/Berlin Difficulty Bomb Delay
    if (_bi.number().asBigInt() >= _chainParams.muirGlacierForkBlock)
    {
        if (exponentialIceAgeBlockNumber >= 9000000)
            exponentialIceAgeBlockNumber -= 9000000;
        else
            exponentialIceAgeBlockNumber = 0;
    }
    // EIP-1234 Constantinople Ice Age delay
    else if (_bi.number().asBigInt() >= _chainParams.constantinopleForkBlock)
    {
        if (exponentialIceAgeBlockNumber >= 5000000)
            exponentialIceAgeBlockNumber -= 5000000;
        else
            exponentialIceAgeBlockNumber = 0;
    }
    // EIP-649 Byzantium Ice Age delay
    else if (_bi.number().asBigInt() >= _chainParams.byzantiumForkBlock)
    {
        if (exponentialIceAgeBlockNumber >= 3000000)
            exponentialIceAgeBlockNumber -= 3000000;
        else
            exponentialIceAgeBlockNumber = 0;
    }

    unsigned periodCount = exponentialIceAgeBlockNumber / c_expDiffPeriod;
    // latter will eventually become huge, so ensure it's a bigint.
    if (periodCount > 1)
        o += VALUE((bigint(1) << (periodCount - 2)));

    o = max<bigint>(minimumDifficulty, o.asBigInt());
    return o;  // bigint(min<bigint>(o, std::numeric_limits<bigint>::max()));
}


VALUE calculateEIP1559BaseFee(ChainOperationParams const& _chainParams, spBlockHeader const& _bi, spBlockHeader const& _parent)
{
    if (_bi->number().asBigInt() == _chainParams.londonForkBlock)
        return INITIAL_BASE_FEE;

    VALUE expectedBaseFee(0);
    BlockHeader1559 const& parent = BlockHeader1559::castFrom(_parent);
    VALUE const parentGasTarget = parent.gasLimit() / ELASTICITY_MULTIPLIER;

    if (parent.gasUsed() == parentGasTarget)
        expectedBaseFee = parent.baseFee().asBigInt();
    else if (parent.gasUsed() > parentGasTarget)
    {
        VALUE gasUsedDelta = parent.gasUsed() - parentGasTarget;
        VALUE formula = parent.baseFee() * gasUsedDelta / parentGasTarget / BASE_FEE_MAX_CHANGE_DENOMINATOR;
        VALUE baseFeePerGasDelta = max<dev::bigint>(formula.asBigInt(), dev::bigint(1));
        expectedBaseFee = parent.baseFee() + baseFeePerGasDelta;
    }
    else
    {
        VALUE gasUsedDelta = parentGasTarget - parent.gasUsed();
        VALUE baseFeePerGasDelta = parent.baseFee() * gasUsedDelta / parentGasTarget / BASE_FEE_MAX_CHANGE_DENOMINATOR;
        if (parent.baseFee() > baseFeePerGasDelta)
            expectedBaseFee = parent.baseFee() - baseFeePerGasDelta;
        else
            expectedBaseFee = 0;
    }
    return expectedBaseFee;
}

}  // namespace toolimpl
