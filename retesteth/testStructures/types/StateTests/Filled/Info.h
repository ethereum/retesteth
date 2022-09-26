#pragma once
#include <libdataobj/DataObject.h>
#include <libdataobj/SPointer.h>
#include <map>

namespace test::teststruct
{
using namespace dataobject;
struct Info : GCP_SPointerBase
{
    Info(DataObject const&);
    std::map<std::string, std::string> const& labels() const { return m_labels; }

private:
    Info() {}
    std::map<std::string, std::string> m_labels;
};

typedef GCP_SPointer<Info> spInfo;

}  // namespace teststruct
