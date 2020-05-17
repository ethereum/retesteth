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
        GCP_SPointer<VALUE> key = GCP_SPointer<VALUE>(new VALUE(tmpKey));
        GCP_SPointer<VALUE> val = GCP_SPointer<VALUE>(new VALUE(el));
        m_map[key.getCPtr()->asString()] = {key, val};
    }
}

DataObject Storage::asDataObject() const
{
    DataObject out;
    for (auto const& el : m_map)
    {
        StorageRecord const& record = el.second;
        out[std::get<0>(record).getCPtr()->asString()] = std::get<1>(record).getCPtr()->asString();
    }
    return out;
}


}  // namespace teststruct
}  // namespace test
