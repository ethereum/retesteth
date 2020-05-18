#pragma once
#include <retesteth/dataObject/DataObject.h>
#include <retesteth/dataObject/SPointer.h>
using namespace dataobject;

namespace test
{
namespace teststruct
{
// FORK network configuration (string wrapper)
struct FORK : GCP_SPointerBase
{
    FORK(std::string const& _s) : m_data(_s) {}
    FORK(DataObject const&);
    string const& asString() const { return m_data.asString(); }

    // std container operations
    inline bool operator==(FORK const& rhs) const { return asString() == rhs.asString(); }
    inline bool operator!=(FORK const& rhs) const { return !(*this == rhs); }
    inline bool operator<(FORK const& rhs) const { return asString() < rhs.asString(); }

private:
    FORK() {}
    DataObject m_data;
};

}  // namespace teststruct
}  // namespace test
