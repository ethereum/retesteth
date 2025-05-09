#pragma once
#include <retesteth/testStructures/basetypes.h>
#include <libdataobj/DataObject.h>
namespace test
{
namespace teststruct
{
// Account Storage  "0x11" -> {value("0x11"),  value("0x1122334455..32") }
// TODO review the logic here
struct Storage : GCP_SPointerBase
{
    Storage(DataObject const&);
    typedef std::tuple<spVALUE, spVALUE> StorageRecord;

    std::map<std::string, StorageRecord> const& getKeys() const { return m_map; }
    bool hasKey(VALUE const& _key) const { return m_map.count(_key.asString()); }
    void addRecord(StorageRecord const& record);
    VALUE const& atKey(VALUE const& _key) const
    {
        assert(m_map.count(_key.asString()));
        return std::get<1>(m_map.at(_key.asString()));
    }
    spDataObject asDataObject() const;
    void merge(Storage const& _storage);

private:
    std::map<std::string, StorageRecord> m_map;
};

typedef GCP_SPointer<Storage> spStorage;


}  // namespace teststruct
}  // namespace test
