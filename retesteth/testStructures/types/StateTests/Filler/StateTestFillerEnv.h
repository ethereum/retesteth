#pragma once
#include "../Base/StateTestEnvBase.h"
#include <retesteth/dataObject/DataObject.h>
using namespace dataobject;
using namespace test::teststruct;

namespace test
{
namespace teststruct
{
// Filler constructor of Env converts all dec fields to hex
// In State test, Env define timestamp of the first block
struct StateTestFillerEnv : StateTestEnvBase
{
    VALUE const& currentTimestamp() const override { return m_genesisTimestamp; }
    VALUE const& firstBlockTimestamp() const { return m_currentTimestamp; }

protected:
    void initializeCommonFields(spDataObject const&);

private:
    spVALUE m_genesisTimestamp;
};

struct StateTestFillerEnv1559 : StateTestFillerEnv, StateTestEnvBase1559
{
    TestEnvClass type() const override { return TestEnvClass::EIP1559; }
    StateTestFillerEnv1559(spDataObjectMove);

protected:
    void initialize1559Fields(DataObject const&) override;
};

struct StateTestFillerEnvLegacy : StateTestFillerEnv, StateTestEnvBaseLegacy
{
    TestEnvClass type() const override { return TestEnvClass::LEGACY; }
    StateTestFillerEnvLegacy(spDataObjectMove);

protected:
    void initializeLegacyFields(DataObject const&) override;
};


}  // namespace teststruct
}  // namespace test
