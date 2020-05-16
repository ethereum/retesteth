#pragma once
#include <retesteth/dataObject/DataObject.h>
#include <retesteth/dataObject/SPointer.h>
using namespace dataobject;

namespace test
{
namespace teststruct
{
// Validate and manage the type of VALUE (bigInt)
// Deserialized from string of "0x1122...32"
// Can be limited by _limit max value

struct VALUE : GCP_SPointerBase
{
    VALUE() {}
    VALUE(DataObject const&,
        dev::u256 _limit = dev::u256(
            "0xffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff"));
    string asString() const { return m_data.asString(); }

private:
    DataObject m_data;
};


}  // namespace teststruct
}  // namespace test
