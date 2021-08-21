#pragma once
#include <libdevcore/Common.h>
#include <libdevcore/CommonData.h>
#include <libdevcore/RLP.h>
#include <retesteth/dataObject/DataObject.h>
#include <retesteth/dataObject/SPointer.h>
using namespace dataobject;

namespace test
{
namespace teststruct
{

enum class ExportType
{
    RLP,
    TEST
};

struct FH : GCP_SPointerBase
{
    FH(dev::RLP const& _rlp, size_t _scale);
    FH(string const&, size_t _scale);
    FH(DataObject const&, size_t _scale);  // Does not require to move smart pointer here as this structure changes a lot
    FH(dev::bigint const&, size_t _scale);

    bool isBigInt() const { return m_bigint; }
    string const& asString(ExportType _forRLP = ExportType::TEST) const;
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
    mutable string m_dataStrZeroXCache;
    mutable string m_dataStrBigIntCache;
    size_t m_scale;
};

}  // namespace teststruct
}  // namespace test
