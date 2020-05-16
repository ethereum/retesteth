#include "Storage.h"

namespace test
{
namespace teststruct
{
Storage::Storage(DataObject const& _data) : m_map(_data) {}

std::list<VALUE> Storage::getKeys() const
{
    std::list<VALUE> keys;
    for (auto const& el : m_map.getSubObjects())
        keys.push_back(VALUE(el));
    return keys;
}

VALUE Storage::getValue(VALUE const& _key) const
{
    return VALUE(m_map.atKey(_key.asString()));
}


}  // namespace teststruct
}  // namespace test
