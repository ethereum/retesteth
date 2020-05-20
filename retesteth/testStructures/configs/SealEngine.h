#pragma once
#include <string>

namespace test
{
namespace teststruct
{

// SealEngine enum
enum class SealEngine
{
    NoProof,
    NoReward,
    Ethash
};

SealEngine sealEngineFromStr(std::string const&);
std::string sealEngineToStr(SealEngine);


}  // namespace teststruct
}  // namespace test
