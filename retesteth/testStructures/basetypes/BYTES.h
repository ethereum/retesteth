#pragma once
#include <retesteth/dataObject/DataObject.h>
#include <retesteth/dataObject/SPointer.h>
using namespace dataobject;

namespace test
{
namespace teststruct
{
// Validate and manage the type of BYTES
// Deserialized from string of "0x1122....." of _limit range
struct BYTES : GCP_SPointerBase
{
    BYTES() {}
    BYTES(DataObject const&);

private:
    DataObject m_data;
};


}  // namespace teststruct
}  // namespace test
