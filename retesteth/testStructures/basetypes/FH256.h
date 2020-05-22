#pragma once
#include "FH.h"
#include <retesteth/dataObject/DataObject.h>
#include <retesteth/dataObject/SPointer.h>
using namespace dataobject;

namespace test
{
namespace teststruct
{
struct FH256 : FH
{
    FH256(DataObject const& _data) : FH(_data, 256) {}
    FH256(string const& _data) : FH(_data, 256) {}
};

typedef GCP_SPointer<FH256> spFH256;

}  // namespace teststruct
}  // namespace test
