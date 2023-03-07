#pragma once
#include "FH.h"
#include <libdataobj/DataObject.h>
namespace test::teststruct
{

// Validate and manage the type of FixedHash32
// Deserialized from string of "0x1122...32" exact length
struct FH32 : FH
{
    FH32(dev::RLP const& _rlp) : FH(_rlp, 32) {}
    FH32(DataObject const& _data) : FH(_data, 32) {}
    FH32(std::string const& _data) : FH(_data, 32) {}
    FH32* copy() const { return new FH32(asString()); }

    bool isZero() const { return m_data.asString() == zero().asStringBytes(); }
    static FH32 const& zero();
};

typedef GCP_SPointer<FH32> spFH32;

}  // namespace teststruct
