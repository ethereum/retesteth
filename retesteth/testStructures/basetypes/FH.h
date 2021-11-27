#pragma once
#include "BYTES.h"
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

struct FH : GCP_SPointerBase
{
    FH(dev::RLP const& _rlp, size_t _scale);
    FH(string const&, size_t _scale);
    FH(DataObject const&, size_t _scale);  // Does not require to move smart pointer here as this structure changes a lot

    string const& asString() const;
    dev::bytes const& serializeRLP() const;
    string const& asStringBytes() const { return m_data.asString(); }
    bool operator==(FH const& rhs) const { return m_data.asString() == rhs.asStringBytes(); }
    bool operator!=(FH const& rhs) const { return m_data.asString() != rhs.asStringBytes(); }
    bool operator<(FH const& rhs) const { return m_data.asString() < rhs.asStringBytes(); }

    size_t scale() const { return m_scale; }

private:
    FH() {}
    //FH(FH const&) {}
    void _initialize(string const& _s, string const& _k = string());

protected:
    BYTES m_data = BYTES(DataObject("0x00"));
    size_t m_scale;
    bool m_isCorrectHash = true;
    mutable string m_dataStrZeroXCache;
    mutable dev::bytes m_rlpDataCache;
};

}  // namespace teststruct
}  // namespace test
