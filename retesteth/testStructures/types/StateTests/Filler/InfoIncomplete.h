#pragma once
#include <libdataobj/DataObject.h>
#include <libdataobj/SPointer.h>

namespace test::teststruct
{
using namespace dataobject;
struct InfoIncomplete : GCP_SPointerBase
{
    InfoIncomplete(spDataObjectMove);
    spDataObject const& rawData() const { return m_rawData; }

private:
    spDataObject m_rawData;
};

typedef GCP_SPointer<InfoIncomplete> spInfoIncomplete;

}  // namespace teststruct
