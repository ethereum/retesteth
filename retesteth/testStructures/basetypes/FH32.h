#pragma once
#include "FH.h"
#include <retesteth/dataObject/DataObject.h>
#include <retesteth/dataObject/SPointer.h>
using namespace dataobject;

namespace test
{
namespace teststruct
{
struct FH32;
typedef GCP_SPointer<FH32> spFH32;
// Validate and manage the type of FixedHash32
// Deserialized from string of "0x1122...32" exact length
struct FH32 : FH
{
    FH32(dev::RLP const& _rlp) : FH(_rlp, 32) {}
    FH32(DataObject const& _data) : FH(_data, 32) {}
    FH32(string const& _data) : FH(_data, 32) {}
    spFH32 copy() const { return spFH32(new FH32(asString())); }

    bool isZero() const { return m_data.asString() == _zero().asStringBytes(); }
    static spFH32 zero()
    {
        spFH32 zero( new FH32("0x0000000000000000000000000000000000000000000000000000000000000000"));
        return zero;
    }

private:
    // Because FH uses BYTES implementation with smart pointer
    // possible memory leak can occure if this is public
    FH32(FH32 const&) : FH("", 0){}
    static FH32 const& _zero()
    {
        static spFH32 szero = zero();
        return szero.getCContent();
    }
};



}  // namespace teststruct
}  // namespace test
