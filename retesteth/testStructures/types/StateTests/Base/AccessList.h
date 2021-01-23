#pragma once
#include <retesteth/dataObject/DataObject.h>
#include <retesteth/dataObject/SPointer.h>
#include <retesteth/testStructures/basetypes.h>
using namespace dataobject;

namespace test
{
namespace teststruct
{
struct AccessListElement : GCP_SPointerBase
{
    AccessListElement(DataObject const&);
    AccessListElement(dev::RLP const& _el);
    FH20 const& address() const { return m_address.getCContent(); }
    std::vector<FH32> const& keys() const { return m_storageKeys; }
    const dev::RLPStream asRLPStream() const;

private:
    spFH20 m_address;
    std::vector<FH32> m_storageKeys;
};

struct AccessList
{
    AccessList(){};
    AccessList(DataObject const&);
    AccessList(dev::RLP const& _rlp);
    DataObject asDataObject() const;
    std::vector<AccessListElement> const& list() const { return m_list; }

private:
    std::vector<AccessListElement> m_list;
};

}  // namespace teststruct
}  // namespace test
