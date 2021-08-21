#pragma once
#include <retesteth/dataObject/DataObject.h>
#include <retesteth/dataObject/SPointer.h>
using namespace dataobject;

namespace test
{
namespace teststruct
{
struct InfoIncomplete : GCP_SPointerBase
{
    InfoIncomplete(spDataObjectMove);
    spDataObject const& rawData() const { return m_rawData; }

private:
    spDataObject m_rawData;
};

typedef GCP_SPointer<InfoIncomplete> spInfoIncomplete;

}  // namespace teststruct
}  // namespace test
