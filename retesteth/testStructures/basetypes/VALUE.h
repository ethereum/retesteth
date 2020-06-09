#pragma once
#include <retesteth/dataObject/DataObject.h>
#include <retesteth/dataObject/SPointer.h>
using namespace dataobject;

namespace test
{
namespace teststruct
{
// Validate and manage the type of VALUE (bigInt)
// Deserialized from string of "0x1122...32", "123343"
// Can be limited by _limit max value

struct VALUE : GCP_SPointerBase
{
    VALUE(dev::u256, dev::u256 const& _limit = dev::u256("0xffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff"));
    VALUE(int, dev::u256 const& _limit = dev::u256("0xffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff"));
    VALUE(DataObject const&,
        dev::u256 const& _limit = dev::u256("0xffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff"));
    bool operator<(int _rhs) const { return m_data < _rhs; }
    bool operator!=(VALUE const& _rhs) const { return m_data != _rhs.asU256(); }
    bool operator==(VALUE const& _rhs) const { return m_data == _rhs.asU256(); }
    VALUE operator-(VALUE const& _rhs) const { return VALUE(m_data - _rhs.asU256()); }
    VALUE operator+(VALUE const& _rhs) const { return VALUE(m_data + _rhs.asU256()); }

    string asString() const { return dev::toCompactHexPrefixed(m_data, 1); }
    string asDecString() const;
    dev::u256 asU256() const { return m_data; }

private:
    VALUE() {}
    void verifyHexString(std::string const& _s, std::string const& _k = string()) const;
    void checkLimit(dev::u256 const& _limit) const;
    dev::u256 m_data;
};

typedef GCP_SPointer<VALUE> spVALUE;


}  // namespace teststruct
}  // namespace test
