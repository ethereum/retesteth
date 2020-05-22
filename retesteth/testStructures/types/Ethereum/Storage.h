#pragma once
#include "../../basetypes.h"
#include <retesteth/dataObject/DataObject.h>
#include <retesteth/dataObject/SPointer.h>
using namespace dataobject;
using namespace test::teststruct;

namespace test
{
namespace teststruct
{
// Account Storage  "0x11" -> "0x1122334455..32"
struct Storage : GCP_SPointerBase
{
    Storage(DataObject const&);
    typedef std::tuple<GCP_SPointer<VALUE>, GCP_SPointer<VALUE>> StorageRecord;

    std::map<string, StorageRecord> const& getKeys() const { return m_map; }
    bool hasKey(VALUE const& _key) const { return m_map.count(_key.asString()); }
    VALUE const& atKey(VALUE const& _key) const { return std::get<1>(m_map.at(_key.asString())).getCContent(); }
    DataObject asDataObject() const;
    void merge(Storage const& _storage);

private:
    std::map<string, StorageRecord> m_map;
};

typedef GCP_SPointer<Storage> spStorage;


}  // namespace teststruct
}  // namespace test
