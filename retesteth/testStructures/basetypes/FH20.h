#pragma once
#include "FH.h"
#include <retesteth/dataObject/DataObject.h>
#include <retesteth/dataObject/SPointer.h>
using namespace dataobject;

namespace test
{
namespace teststruct
{
// Validate and manage the type of FixedHash32
// Deserialized from string of "0x1122...20" exact length
struct FH20 : FH
{
    FH20(DataObject const& _data) : FH(_data, 20){};
};

typedef GCP_SPointer<FH20> spFH20;

}  // namespace teststruct
}  // namespace test
