#pragma once
#include "AccountIncomplete.h"
#include "Base/StateBase.h"
#include <libdataobj/DataObject.h>
#include <libdataobj/SPointer.h>

namespace test
{
namespace teststruct
{
// Marks that State is made of AccountIncomplete
struct StateIncomplete : StateBase
{
    StateIncomplete(spDataObjectMove);
    void correctMiningReward(FH20 const& _coinbase, VALUE const& _reward);
    spDataObject const& asDataObject() const override;
    spDataObject const& rawData() const { return m_rawData; }

private:
    StateIncomplete(){};
    spDataObject m_rawData;
};

typedef GCP_SPointer<StateIncomplete> spStateIncomplete;

}  // namespace teststruct
}  // namespace test
