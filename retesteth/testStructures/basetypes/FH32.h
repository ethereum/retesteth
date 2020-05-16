#pragma once
#include <retesteth/dataObject/DataObject.h>
#include <retesteth/dataObject/SPointer.h>
using namespace dataobject;

namespace test
{
namespace teststruct
{
// Validate and manage the type of FixedHash32
// Deserialized from string of "0x1122...32" exact length
struct FH32 : GCP_SPointerBase
{
    FH32() {}
    FH32(DataObject const&);

private:
    DataObject m_data;
};

}  // namespace teststruct
}  // namespace test
