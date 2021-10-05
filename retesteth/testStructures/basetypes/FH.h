#pragma once
#include "VALUE.h"
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

    bool isBigInt() const { return m_data->isBigInt(); }
    string const& asString() const;
    dev::bytes const& serializeRLP() const;
    dev::bigint const& asBigInt() const { return m_data->asBigInt(); }
    bool operator==(FH const& rhs) const { return asBigInt() == rhs.asBigInt(); }
    bool operator!=(FH const& rhs) const { return asBigInt() != rhs.asBigInt(); }
    bool operator<(FH const& rhs) const { return asBigInt() < rhs.asBigInt(); }

    size_t scale() const { return m_scale; }

private:
    FH() {}
    void _initialize(string const& _s, string const& _k = string());

protected:
    spVALUE m_data;
    size_t m_scale;
    bool m_isCorrectHash = true;
    mutable string m_dataStrZeroXCache;
    mutable dev::bytes m_rlpDataCache;
};

}  // namespace teststruct
}  // namespace test
