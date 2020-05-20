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
    VALUE(DataObject const&,
        dev::u256 _limit = dev::u256(
            "0xffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff"));
    string asString() const { return m_data.asString(); }
    string asDecString() const;
    dev::u256 asU256() const { return dev::u256(m_data.asString()); }

private:
    VALUE() {}
    DataObject m_data;
};

typedef GCP_SPointer<VALUE> spVALUE;


}  // namespace teststruct
}  // namespace test
