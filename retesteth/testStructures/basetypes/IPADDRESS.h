#pragma once
#include <libdataobj/DataObject.h>

namespace test::teststruct
{
using namespace dataobject;
// Validate and manage 127.0.0.1:8080 (string wrapper)
struct IPADDRESS : GCP_SPointerBase
{
    IPADDRESS(DataObject const&);
    std::string const& asString() const { return m_data; }

    // std container operations
    inline bool operator==(IPADDRESS const& rhs) const { return asString() == rhs.asString(); }
    inline bool operator!=(IPADDRESS const& rhs) const { return !(*this == rhs); }
    inline bool operator<(IPADDRESS const& rhs) const { return asString() < rhs.asString(); }

private:
    IPADDRESS() {}
    std::string m_data;
};

typedef GCP_SPointer<IPADDRESS> spIPADDRESS;

}  // namespace teststruct
