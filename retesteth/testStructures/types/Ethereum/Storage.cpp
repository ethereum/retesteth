#include "Storage.h"

using namespace std;
namespace test::teststruct
{
Storage::Storage(DataObject const& _data)
{
    for (auto const& el : _data.getSubObjects())
    {
        DataObject tmpKey;
        tmpKey.setKey("Storage record in storage");  // Hint
        tmpKey.setString(string(el->getKey()));
        spVALUE key(new VALUE(tmpKey));
        spVALUE val(new VALUE(el));
        m_map[key->asString()] = {key, val};
    }
}

void Storage::merge(Storage const& _storage)
{
    // same keys???
    for (auto const& record : _storage.getKeys())
        m_map[record.first] = record.second;
}

spDataObject Storage::asDataObject() const
{
    spDataObject out(new DataObject(DataType::Object));
    for (auto const& el : m_map)
    {
        StorageRecord const& record = el.second;
        (*out)[std::get<0>(record)->asString()] = std::get<1>(record)->asString();
    }
    return out;
}

}  // namespace teststruct
