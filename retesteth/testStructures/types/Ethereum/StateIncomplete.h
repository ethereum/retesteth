#pragma once
#include "Base/StateBase.h"
#include <libdataobj/DataObject.h>

namespace test::teststruct
{
// Marks that State is made of AccountIncomplete
struct StateIncomplete : StateBase
{
    StateIncomplete(spDataObjectMove);
    void correctMiningReward(FH20 const& _coinbase, VALUE const& _reward);
    spDataObject asDataObject() const override;

private:
    StateIncomplete(){};
};

typedef GCP_SPointer<StateIncomplete> spStateIncomplete;

}  // namespace teststruct
