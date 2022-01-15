#include "StateTestEnvBase.h"
#include <retesteth/EthChecks.h>
namespace test
{
namespace teststruct
{
StateTestEnvBaseMerge const* StateTestEnvBaseMerge::castFrom(StateTestEnvBase const* _from)
{
    try
    {
        if (_from->type() != TestEnvClass::MERGE)
            ETH_FAIL_MESSAGE("StateTestEnvBaseMERGE::castFrom() got wrong type!");
        return dynamic_cast<StateTestEnvBaseMerge const*>(_from);
    }
    catch (...)
    {
        ETH_FAIL_MESSAGE("StateTestEnvBaseMerge::castFrom() failed!");
    }
    return 0;
}

StateTestEnvBase1559 const* StateTestEnvBase1559::castFrom(StateTestEnvBase const* _from)
{
    try
    {
        if (_from->type() != TestEnvClass::EIP1559)
            ETH_FAIL_MESSAGE("StateTestEnvBase1559::castFrom() got wrong type!");
        return dynamic_cast<StateTestEnvBase1559 const*>(_from);
    }
    catch (...)
    {
        ETH_FAIL_MESSAGE("StateTestEnvBase1559::castFrom() failed!");
    }
    return 0;
}

StateTestEnvBaseLegacy const* StateTestEnvBaseLegacy::castFrom(StateTestEnvBase const* _from)
{
    try
    {
        if (_from->type() != TestEnvClass::LEGACY)
            ETH_FAIL_MESSAGE("StateTestEnvBaseLegacy::castFrom() got wrong type!");
        return dynamic_cast<StateTestEnvBaseLegacy const*>(_from);
    }
    catch (...)
    {
        ETH_FAIL_MESSAGE("StateTestEnvBaseLegacy::castFrom() failed!");
    }
    return 0;
}

spDataObject const& StateTestEnvBase::asDataObject() const
{
    return m_raw;
}

}  // namespace teststruct
}  // namespace test
