#include "Storage.h"

namespace test
{
namespace teststruct
{
Storage::Storage(DataObject const& _data)
{
    for (auto const& el : _data.getSubObjects())
    {
        DataObject tmpKey;
        tmpKey.setKey("Storage record in storage");  // Hint
        tmpKey.setString(el.getKey());
        spVALUE key(new VALUE(tmpKey));
        spVALUE val(new VALUE(el));
        m_map[key.getCContent().asString()] = {key, val};
    }
}

void Storage::merge(Storage const& _storage)
{
    // same keys???
    for (auto const& record : _storage.getKeys())
        m_map[record.first] = record.second;
}

const DataObject Storage::asDataObject() const
{
    DataObject out(DataType::Object);
    for (auto const& el : m_map)
    {
        StorageRecord const& record = el.second;
        out[std::get<0>(record).getCContent().asString()] = std::get<1>(record).getCContent().asString();
    }
    return out;
}


}  // namespace teststruct
}  // namespace test
