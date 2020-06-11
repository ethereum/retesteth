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
    StateTestEnv(DataObject const& _data);
    VALUE const& currentTimestamp() const override { return m_genesisTimestamp.getCContent(); }
    VALUE const& firstBlockTimestamp() const { return m_currentTimestamp.getCContent(); }

private:
    spVALUE m_genesisTimestamp;
};


}  // namespace teststruct
}  // namespace test
