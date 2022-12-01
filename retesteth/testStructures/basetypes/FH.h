#pragma once
#include "BYTES.h"
#include <libdevcore/RLP.h>
#include <libdataobj/DataObject.h>

namespace test::teststruct
{
struct FH : GCP_SPointerBase
{
    FH(dev::RLP const& _rlp, size_t _scale);
    FH(std::string const&, size_t _scale);
    FH(DataObject const&, size_t _scale);  // Does not require to move smart pointer here as this structure changes a lot

    std::string const& asString() const;
    dev::bytes const& serializeRLP() const;
    std::string const& asStringBytes() const { return m_data.asString(); }
    bool operator==(FH const& rhs) const { return m_data.asString() == rhs.asStringBytes(); }
    bool operator!=(FH const& rhs) const { return m_data.asString() != rhs.asStringBytes(); }
    bool operator<(FH const& rhs) const { return m_data.asString() < rhs.asStringBytes(); }

    size_t scale() const { return m_scale; }

private:
    FH() {}
    //FH(FH const&) {}
    void _initialize(std::string const& _s, std::string const& _k = std::string());

protected:
    BYTES m_data = BYTES(DataObject("0x00"));
    size_t m_scale;
    bool m_isCorrectHash = true;
    mutable std::string m_dataStrZeroXCache;
    mutable dev::bytes m_rlpDataCache;
};

}  // namespace teststruct
