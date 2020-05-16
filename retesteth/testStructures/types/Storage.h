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
struct Storage : GCP_SPointerBase
{
    Storage(DataObject const&);
    std::list<VALUE> getKeys() const;
    VALUE getValue(VALUE const& _key) const;

private:
    DataObject m_map;
};

}  // namespace teststruct
}  // namespace test
