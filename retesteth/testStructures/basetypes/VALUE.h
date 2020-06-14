#pragma once
#include <libdevcore/Common.h>
#include <libdevcore/CommonData.h>
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
    VALUE(dev::u256);
    VALUE(int);
    VALUE(DataObject const&);
    bool operator<(int _rhs) const { return m_data < _rhs; }
    bool operator>(VALUE const& _rhs) const { return m_data > _rhs.asU256(); }
    bool operator<(VALUE const& _rhs) const { return m_data < _rhs.asU256(); }
    bool operator!=(VALUE const& _rhs) const { return m_data != _rhs.asU256(); }
    bool operator==(VALUE const& _rhs) const { return m_data == _rhs.asU256(); }
    VALUE operator-(VALUE const& _rhs) const { return VALUE(m_data - _rhs.asU256()); }
    VALUE operator+(VALUE const& _rhs) const { return VALUE(m_data + _rhs.asU256()); }

    string asString(size_t _roundBytes = 1) const { return dev::toCompactHexPrefixed(m_data, _roundBytes); }
    string asDecString() const;
    dev::u256 asU256() const { return m_data; }

private:
    VALUE() {}
    void verifyHexString(std::string const& _s, std::string const& _k = string()) const;
    dev::u256 m_data;
};

typedef GCP_SPointer<VALUE> spVALUE;


}  // namespace teststruct
}  // namespace test
