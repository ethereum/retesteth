#include "AccessList.h"
#include <libdevcore/Address.h>
#include <retesteth/testStructures/Common.h>

namespace test
{
namespace teststruct
{
AccessListElement::AccessListElement(DataObject const& _data)
{
    m_address = spFH20(new FH20(_data.atKey("address")));
    for (auto const& el : _data.atKey("storageKeys").getSubObjects())
        m_storageKeys.push_back(spFH32(new FH32(dev::toCompactHexPrefixed(dev::u256(el.asString()), 32))));

    requireJsonFields(_data, "AccessListElement " + _data.getKey(),
        {{"address", {{DataType::String}, jsonField::Required}}, {"storageKeys", {{DataType::Array}, jsonField::Required}}});
}

AccessList::AccessList(DataObject const& _data)
{
    for (auto const& el : _data.getSubObjects())
        m_list.push_back(spAccessListElement(new AccessListElement(el)));
}

DataObject AccessList::asDataObject() const
{
    DataObject accessList(DataType::Array);
    for (auto const& el : m_list)
    {
        DataObject accessListElement;
        accessListElement["address"] = el->address().asString();
        DataObject keys(DataType::Array);
        for (auto const& el2 : el->keys())
            keys.addArrayObject(el2->asString());
        accessListElement["storageKeys"] = keys;
        accessList.addArrayObject(accessListElement);
    }
    return accessList;
}

const dev::RLPStream AccessListElement::asRLPStream() const
{
    dev::RLPStream stream(2);
    stream << dev::Address(m_address->asString());

    dev::RLPStream storages(m_storageKeys.size());
    for (auto const& key : m_storageKeys)
        storages << dev::h256(key->asString());
    // storages << dev::u256(key.asString());

    stream.appendRaw(storages.out());
    return stream;
}

AccessListElement::AccessListElement(dev::RLP const& _rlp)
{
    size_t i = 0;
    m_address = spFH20(new FH20(rlpToString(_rlp[i++])));
    auto const& rlplist = _rlp[i++].toList();
    for (auto const& key : rlplist)
        m_storageKeys.push_back(spFH32(new FH32(rlpToString(key))));
}

AccessList::AccessList(dev::RLP const& _rlp)
{
    for (auto const& accList : _rlp.toList())
        m_list.push_back(spAccessListElement(new AccessListElement(accList)));
}

}  // namespace teststruct
}  // namespace test
