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
    BYTES(DataObject const&);
    string const& asString() const { return m_data; }
    bool operator!=(BYTES const& _rhs) const { return m_data != _rhs.asString(); }
    bool operator==(BYTES const& _rhs) const { return m_data == _rhs.asString(); }

private:
    BYTES() {}
    string m_data;
};

typedef GCP_SPointer<BYTES> spBYTES;

}  // namespace teststruct
}  // namespace test
