#pragma once
#include "FH.h"
#include <retesteth/dataObject/DataObject.h>
#include <retesteth/dataObject/SPointer.h>
using namespace dataobject;

namespace test
{
namespace teststruct
{
struct FH8 : FH
{
    FH8(string const& _data) : FH(_data, 8) {}
    FH8(DataObject const& _data) : FH(_data, 8) {}
    static FH8 zero() { return FH8("0x0000000000000000"); }
};

typedef GCP_SPointer<FH8> spFH8;

}  // namespace teststruct
}  // namespace test
