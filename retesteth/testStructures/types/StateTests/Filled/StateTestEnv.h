#pragma once
#include "../Base/StateTestEnvBase.h"
#include <retesteth/dataObject/DataObject.h>
using namespace dataobject;
using namespace test::teststruct;

namespace test
{
namespace teststruct
{
// Marks that env section is from filled state test
// Constructor require only hex values here
struct StateTestEnv : StateTestEnvBase
{
    VALUE const& currentTimestamp() const override { return m_genesisTimestamp; }
    VALUE const& firstBlockTimestamp() const { return m_currentTimestamp; }

protected:
    void initializeCommonFields(DataObject const&);

private:
    spVALUE m_genesisTimestamp;
};

struct StateTestEnvMerge : StateTestEnv, StateTestEnvBaseMerge
{
    TestEnvClass type() const override { return TestEnvClass::MERGE; }
    StateTestEnvMerge(DataObject const& _data);

protected:
    void initializeMergeFields(DataObject const&) override;
};

struct StateTestEnv1559 : StateTestEnv, StateTestEnvBase1559
{
    TestEnvClass type() const override { return TestEnvClass::EIP1559; }
    StateTestEnv1559(DataObject const& _data);

protected:
    void initialize1559Fields(DataObject const&) override;
};

struct StateTestEnvLegacy : StateTestEnv, StateTestEnvBaseLegacy
{
    TestEnvClass type() const override { return TestEnvClass::LEGACY; }
    StateTestEnvLegacy(DataObject const& _data);

protected:
    void initializeLegacyFields(DataObject const&) override;
};


}  // namespace teststruct
}  // namespace test
