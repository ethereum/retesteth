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
    InfoIncomplete(DataObject const&);
    string const& comment() const { return m_comment; }

private:
    string m_comment;
};

}  // namespace teststruct
}  // namespace test
