#pragma once
#include "../Base/StateTestEnvBase.h"
#include <libdataobj/DataObject.h>

namespace test
{
namespace teststruct
{
// Marks that env section is from filled state test
// Constructor require only hex values here
struct StateTestEnv : StateTestEnvBase
{
    StateTestEnv(DataObject const& _data);
    VALUE const& currentTimestamp() const override { return m_genesisTimestamp; }
    VALUE const& firstBlockTimestamp() const { return m_currentTimestamp; }

protected:
    void initializeFields(DataObject const&);

private:
    spVALUE m_genesisTimestamp;
};

}  // namespace teststruct
}  // namespace test
