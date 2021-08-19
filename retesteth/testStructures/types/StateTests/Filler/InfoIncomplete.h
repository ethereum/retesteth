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
    string const& comment() const { return m_comment; }

private:
    string m_comment;
    spDataObject m_rawData;
};

typedef GCP_SPointer<InfoIncomplete> spInfoIncomplete;

}  // namespace teststruct
}  // namespace test
