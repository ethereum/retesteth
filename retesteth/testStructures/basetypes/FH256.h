#pragma once
#include "FH.h"
#include <libdataobj/DataObject.h>

namespace test::teststruct
{

struct FH256 : FH
{
    FH256(dev::RLP const& _rlp) : FH(_rlp, 256) {}
    FH256(dataobject::DataObject const& _data) : FH(_data, 256) {}
    FH256(std::string const& _data) : FH(_data, 256) {}
    FH256* copy() const;

    static FH256 const& zero();
};

typedef dataobject::GCP_SPointer<FH256> spFH256;
template <class T>
spFH256 sFH256(T const& _arg) { return spFH256(new FH256(_arg)); }


}  // namespace teststruct
