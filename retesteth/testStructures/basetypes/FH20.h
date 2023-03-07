#pragma once
#include "FH.h"
#include <libdataobj/DataObject.h>

namespace test::teststruct
{

// Validate and manage the type of FixedHash20
// Deserialized from string of "0x1122...20" exact length
struct FH20 : FH
{
    FH20(dev::RLP const& _rlp) : FH(_rlp, 20) {}
    FH20(DataObject const& _data) : FH(_data, 20) {}
    FH20(std::string const& _data) : FH(_data, 20) {}
    FH20* copy() const { return new FH20(asString()); }

    static FH20 random();
    static FH20 const& zero();
};

typedef GCP_SPointer<FH20> spFH20;

}  // namespace teststruct
