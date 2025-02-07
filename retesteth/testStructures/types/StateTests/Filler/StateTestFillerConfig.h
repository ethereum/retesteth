#pragma once
#include "../Base/StateTestConfigBase.h"
namespace test::teststruct
{

struct StateTestFillerConfig : StateTestConfigBase
{
    StateTestFillerConfig(std::set<FORK> const& _testForks) : StateTestConfigBase(_testForks){}
};


}  // namespace teststruct
