#pragma once
#include <libdataobj/DataObject.h>
#include <libdataobj/SPointer.h>
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
