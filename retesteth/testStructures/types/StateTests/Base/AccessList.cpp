#include "AccessList.h"
#include <retesteth/testStructures/Common.h>

using namespace std;
namespace test::teststruct
{
AccessListElement::AccessListElement(DataObject const& _data)
{
    REQUIRE_JSONFIELDS(_data, "AccessListElement " + _data.getKey(),
        {{"address", {{DataType::String}, jsonField::Required}},
         {"storageKeys", {{DataType::Array}, jsonField::Required}}});

    m_address = spFH20(new FH20(_data.atKey("address")));
    for (auto const& el : _data.atKey("storageKeys").getSubObjects())
    {
        auto const elStr = el->asString();
        if (elStr.find("bigint") != string::npos)
            m_storageKeys.push_back(spFH32(new FH32(el->asString())));
        else
        {
            // TODO: This is a filler file convertion logic!!!
            m_storageKeys.push_back(spFH32(new FH32(dev::toCompactHexPrefixed(dev::u256(elStr), 32))));
        }
    }
}

AccessList::AccessList(DataObject const& _data)
{
    for (auto const& el : _data.getSubObjects())
        m_list.push_back(spAccessListElement(new AccessListElement(el)));
}

spDataObject AccessList::asDataObject() const
{
    spDataObject accessList(new DataObject(DataType::Array));
    for (auto const& el : m_list)
    {
        spDataObject accessListElement;
        (*accessListElement)["address"] = el->address().asString();
        spDataObject keys(new DataObject(DataType::Array));
        (*accessListElement).atKeyPointer("storageKeys") = keys;
        for (auto const& el2 : el->keys())
        {
            spDataObject k(new DataObject(el2->asString()));
            (*accessListElement)["storageKeys"].addArrayObject(k);
        }
        (*accessList).addArrayObject(accessListElement);
    }
    return accessList;
}

const dev::RLPStream AccessListElement::asRLPStream() const
{
    dev::RLPStream stream(2);
    stream << m_address->serializeRLP();

    dev::RLPStream storages(m_storageKeys.size());
    for (auto const& key : m_storageKeys)
        storages << key->serializeRLP();

    stream.appendRaw(storages.out());
    return stream;
}

AccessListElement::AccessListElement(dev::RLP const& _rlp)
{
    size_t i = 0;
    m_address = spFH20(new FH20(_rlp[i++]));
    auto const& rlplist = _rlp[i++].toList();
    for (auto const& key : rlplist)
        m_storageKeys.push_back(spFH32(new FH32(key)));
}

AccessList::AccessList(dev::RLP const& _rlp)
{
    for (auto const& accList : _rlp.toList())
        m_list.push_back(spAccessListElement(new AccessListElement(accList)));
}

}  // namespace teststruct
