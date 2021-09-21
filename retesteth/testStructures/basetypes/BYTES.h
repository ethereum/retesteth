#pragma once
#include <libdevcore/RLP.h>
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
    BYTES(dev::RLP const& _rlp);
    explicit BYTES(DataObject const&);
    BYTES* copy() const { return new BYTES(DataObject(m_data)); }

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
