#pragma once
#include <libdevcore/Common.h>
#include <libdevcore/CommonData.h>
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
    FH(dev::bigint const&, size_t _scale);

    string asString() const;
    dev::bigint const& asBigInt() const { return m_data; }
    bool operator==(FH const& rhs) const { return asBigInt() == rhs.asBigInt(); }
    bool operator!=(FH const& rhs) const { return asBigInt() != rhs.asBigInt(); }
    bool operator<(FH const& rhs) const { return asBigInt() < rhs.asBigInt(); }

    size_t scale() const { return m_scale; }

private:
    FH() {}
    void _initialize(string const& _s, string const& _k = string());

protected:
    bool m_bigint = false;
    dev::bigint m_data;
    size_t m_scale;
};

}  // namespace teststruct
}  // namespace test
