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
enum class DataRequier
{
    ALLOWDEC,
    ONLYHEX
};
// Marks that State is made of AccountIncomplete
struct StateIncomplete : StateBase
{
    StateIncomplete(DataObject const&, DataRequier req = DataRequier::ONLYHEX);
    void correctMiningReward(FH20 const& _coinbase, VALUE const& _reward);
    DataObject const asDataObject(ExportOrder _order = ExportOrder::Default) const override;

private:
    StateIncomplete(){};
};

}  // namespace teststruct
}  // namespace test
