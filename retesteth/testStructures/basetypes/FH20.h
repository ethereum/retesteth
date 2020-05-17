#pragma once
#include <retesteth/dataObject/DataObject.h>
#include <retesteth/dataObject/SPointer.h>
using namespace dataobject;

namespace test
{
namespace teststruct
{
// Validate and manage the type of FixedHash32
// Deserialized from string of "0x1122...20" exact length
struct FH20 : GCP_SPointerBase
{
    FH20(DataObject const&);
    string const& asString() const { return m_data.asString(); }

private:
    FH20() {}
    DataObject m_data;
};

}  // namespace teststruct
}  // namespace test
