#pragma once
#include "../Base/StateTestConfigBase.h"
namespace test
{
namespace teststruct
{
struct StateTestConfig : StateTestConfigBase
{
    StateTestConfig(DataObject const&, std::set<FORK> const&);
    ~StateTestConfig() override {}
private:
    void initializeFields(DataObject const& _data, std::set<FORK> const&);
};

}  // namespace teststruct
}  // namespace test
