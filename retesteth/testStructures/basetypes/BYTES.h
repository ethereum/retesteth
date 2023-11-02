#pragma once
#include <libdevcore/RLP.h>
#include <libdataobj/DataObject.h>

namespace test::teststruct
{

// Validate and manage the type of BYTES
// Deserialized from string of "0x1122....." of _limit range
struct BYTES : dataobject::GCP_SPointerBase
{
    explicit BYTES(dev::RLP const& _rlp);
    explicit BYTES(dataobject::DataObject const&);
    explicit BYTES(std::string const&);
    BYTES* copy() const { return new BYTES(m_data); }

    std::string const& asString() const { return m_data; }
    std::string& asStringUnsafe() { return m_data; }
    size_t firstByte() const;
    bool operator!=(BYTES const& _rhs) const { return m_data != _rhs.asString(); }
    bool operator==(BYTES const& _rhs) const { return m_data == _rhs.asString(); }

private:
    BYTES() {}
    std::string m_data;
};

typedef dataobject::GCP_SPointer<BYTES> spBYTES;
template <class T>
spBYTES sBYTES(T const& _arg) { return spBYTES(new BYTES(_arg)); }


}  // namespace teststruct
