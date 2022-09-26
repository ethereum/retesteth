#pragma once
#include "FH.h"
#include <libdataobj/DataObject.h>
#include <libdataobj/SPointer.h>

namespace test
{
namespace teststruct
{
struct FH8 : FH
{
    FH8(dev::RLP const& _rlp) : FH(_rlp, 8) {}
    FH8(std::string const& _data) : FH(_data, 8) {}
    FH8(DataObject const& _data) : FH(_data, 8) {}
    FH8* copy() const { return new FH8(asString()); }

    static FH8 const& zero()
    {
        static FH8 zero("0x0000000000000000");
        return zero;
    }
};

typedef GCP_SPointer<FH8> spFH8;

}  // namespace teststruct
}  // namespace test
