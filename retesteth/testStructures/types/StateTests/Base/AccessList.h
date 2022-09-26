#pragma once
#include <libdataobj/DataObject.h>
#include <libdataobj/SPointer.h>
#include <retesteth/testStructures/basetypes.h>

namespace test
{
namespace teststruct
{
struct AccessListElement : GCP_SPointerBase
{
    AccessListElement(DataObject const&);
    AccessListElement(dev::RLP const& _el);
    FH20 const& address() const { return m_address; }
    std::vector<spFH32> const& keys() const { return m_storageKeys; }
    const dev::RLPStream asRLPStream() const;

private:
    spFH20 m_address;
    std::vector<spFH32> m_storageKeys;
};

typedef GCP_SPointer<AccessListElement> spAccessListElement;

struct AccessList : GCP_SPointerBase
{
    AccessList(){};
    AccessList(DataObject const&);
    AccessList(dev::RLP const& _rlp);
    spDataObject asDataObject() const;
    std::vector<spAccessListElement> const& list() const { return m_list; }

private:
    std::vector<spAccessListElement> m_list;
};

typedef GCP_SPointer<AccessList> spAccessList;

}  // namespace teststruct
}  // namespace test
