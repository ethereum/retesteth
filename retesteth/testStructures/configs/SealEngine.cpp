#include "SealEngine.h"
#include <retesteth/EthChecks.h>
using namespace test::teststruct;

namespace test
{
namespace teststruct
{

SealEngine sealEngineFromStr(std::string const& str)
{
    if (str == "NoReward")
        return SealEngine::NoReward;
    if (str == "NoProof")
        return SealEngine::NoProof;
    if (str == "Ethash")
        return SealEngine::Ethash;
    ETH_ERROR_MESSAGE("Failed to read SealEngine from str: `" + str + "`");
    return SealEngine::NoProof;
}

std::string sealEngineToStr(SealEngine _engine)
{
    switch (_engine) {
    case SealEngine::Ethash:
        return "Ethash";
        break;
    case SealEngine::NoProof:
        return "NoProof";
        break;
    case SealEngine::NoReward:
        return "NoReward";
        break;
    default:
        break;
    }
    return std::string();
}

}  // namespace teststruct
}  // namespace test
