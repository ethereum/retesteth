#pragma once
#include "FH.h"
#include <retesteth/dataObject/DataObject.h>
#include <retesteth/dataObject/SPointer.h>
using namespace dataobject;

namespace test
{
namespace teststruct
{
struct FH8 : FH
{
    FH8(string const& _data) : FH(_data, 8) {}
    FH8(DataObject const& _data) : FH(_data, 8) {}
    FH8(dev::bigint const& _data) : FH(_data, 8) {}
    FH8* copy() const { return new FH8(m_data); }

    static FH8 const& zero()
    {
        static FH8 zero("0x0000000000000000");
        return zero;
    }
};

typedef GCP_SPointer<FH8> spFH8;

}  // namespace teststruct
}  // namespace test
