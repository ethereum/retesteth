#pragma once
#include "AccountIncomplete.h"
#include "Base/StateBase.h"
#include <retesteth/dataObject/DataObject.h>
#include <retesteth/dataObject/SPointer.h>
using namespace dataobject;
using namespace test::teststruct;

namespace test
{
namespace teststruct
{
// Marks that State is made of AccountIncomplete
struct StateIncomplete : StateBase
{
    StateIncomplete(DataObject const&);
    void correctMiningReward(FH20 const& _coinbase, VALUE const& _reward);
    DataObject const asDataObject() const override;

private:
    StateIncomplete(){};
};

}  // namespace teststruct
}  // namespace test
