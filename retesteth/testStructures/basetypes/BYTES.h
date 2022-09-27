#pragma once
#include <libdevcore/RLP.h>
#include <libdataobj/DataObject.h>

namespace test::teststruct
{
using namespace dataobject;

// Validate and manage the type of BYTES
// Deserialized from string of "0x1122....." of _limit range
struct BYTES : GCP_SPointerBase
{
    explicit BYTES(dev::RLP const& _rlp);
    explicit BYTES(DataObject const&);
    BYTES* copy() const { return new BYTES(DataObject(m_data)); }

    std::string const& asString() const { return m_data; }
    size_t firstByte() const;
    bool operator!=(BYTES const& _rhs) const { return m_data != _rhs.asString(); }
    bool operator==(BYTES const& _rhs) const { return m_data == _rhs.asString(); }

private:
    BYTES() {}
    std::string m_data;
};

typedef GCP_SPointer<BYTES> spBYTES;

}  // namespace teststruct
