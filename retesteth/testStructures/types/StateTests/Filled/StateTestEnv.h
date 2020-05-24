#pragma once
#include "../../../basetypes/BYTES.h"
#include "../../../basetypes/FH20.h"
#include "../../../basetypes/FH32.h"
#include "../../../basetypes/VALUE.h"
#include "../Base/StateTestEnvBase.h"
#include <retesteth/dataObject/DataObject.h>
#include <retesteth/dataObject/SPointer.h>
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
};


}  // namespace teststruct
}  // namespace test
