#pragma once
#include <retesteth/dataObject/DataObject.h>
#include <retesteth/dataObject/SPointer.h>
using namespace dataobject;

namespace test
{
namespace teststruct
{
struct FH : GCP_SPointerBase
{
    FH(string const&, size_t _scale);
    FH(DataObject const&, size_t _scale);

    string const& asString() const { return m_data; }
    bool operator==(FH const& rhs) const { return asString() == rhs.asString(); }
    bool operator!=(FH const& rhs) const { return asString() != rhs.asString(); }
    bool operator<(FH const& rhs) const { return asString() < rhs.asString(); }

    size_t scale() const { return m_scale; }

private:
    FH() {}

protected:
    string m_data;
    size_t m_scale;
};

}  // namespace teststruct
}  // namespace test
