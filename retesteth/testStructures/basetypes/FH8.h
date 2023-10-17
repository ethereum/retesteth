#pragma once
#include "FH.h"
#include <libdataobj/DataObject.h>

namespace test::teststruct
{

struct FH8 : FH
{
    FH8(dev::RLP const& _rlp) : FH(_rlp, 8) {}
    FH8(std::string const& _data) : FH(_data, 8) {}
    FH8(dataobject::DataObject const& _data) : FH(_data, 8) {}
    FH8* copy() const;
    static FH8 const& zero();
};

typedef dataobject::GCP_SPointer<FH8> spFH8;
template <class T>
spFH8 sFH8(T const& _arg) { return spFH8(new FH8(_arg)); }


}  // namespace teststruct
