#pragma once
#include <libdataobj/DataObject.h>
#include <libdataobj/SPointer.h>
#include <map>
using namespace dataobject;

namespace test
{
namespace teststruct
{
struct Info : GCP_SPointerBase
{
    Info(DataObject const&);
    std::map<string, string> const& labels() const { return m_labels; }

private:
    Info() {}
    std::map<string, string> m_labels;
};

typedef GCP_SPointer<Info> spInfo;

}  // namespace teststruct
}  // namespace test
