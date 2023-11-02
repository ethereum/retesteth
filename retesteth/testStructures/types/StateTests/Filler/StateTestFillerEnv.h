#pragma once
#include "../Base/StateTestEnvBase.h"
#include <libdataobj/DataObject.h>

namespace test::teststruct
{
// Filler constructor of Env converts all dec fields to hex
// In State test, Env define timestamp of the first block
struct StateTestFillerEnv : StateTestEnvBase
{
    StateTestFillerEnv(spDataObjectMove);
    VALUE const& currentTimestamp() const override { return m_genesisTimestamp; }
    VALUE const& firstBlockTimestamp() const { return m_currentTimestamp; }
    spDataObject const& asDataObject() const override;

protected:
    void initializeFields(spDataObject const&);

private:
    spVALUE m_genesisTimestamp;
};


}  // namespace teststruct
