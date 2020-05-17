#pragma once
#include "../basetypes/VALUE.h"
#include "Storage.h"
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
    DataObject asDataObject() const;

private:
    std::map<string, StorageRecord> m_map;
};


}  // namespace teststruct
}  // namespace test
