#pragma once
#include "FH.h"
#include <libdataobj/DataObject.h>

namespace test::teststruct
{

struct FH256 : FH
{
    FH256(dev::RLP const& _rlp) : FH(_rlp, 256) {}
    FH256(DataObject const& _data) : FH(_data, 256) {}
    FH256(std::string const& _data) : FH(_data, 256) {}
    FH256* copy() const { return new FH256(asString()); }

    static FH256 const& zero();
};

typedef GCP_SPointer<FH256> spFH256;

}  // namespace teststruct
