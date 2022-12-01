#pragma once
#include <libdataobj/DataObject.h>
#include <libdevcore/Common.h>
#include <libdevcore/RLP.h>

namespace test::teststruct
{
using namespace dataobject;

// Validate and manage the type of VALUE (bigInt)
// Deserialized from string of "0x1122...32", "123343"
// Can be limited by _limit max value

struct VALUE : GCP_SPointerBase
{
    // TODO ideally separate bigint logic into another class that behave exactly the same as VALUE but with exceptions
    VALUE(dev::RLP const& _rlp);
    VALUE(dev::bigint const&);
    VALUE(int);
    VALUE(DataObject const&);  // Does not require to move smart pointer here as this structure changes a lot
    VALUE* copy() const { return new VALUE(m_data); }

    bool operator<(long long _rhs) const { return m_data < _rhs; }
    bool operator>(VALUE const& _rhs) const { return m_data > _rhs.asBigInt(); }
    bool operator>=(VALUE const& _rhs) const { return m_data >= _rhs.asBigInt(); }
    bool operator<(VALUE const& _rhs) const { return m_data < _rhs.asBigInt(); }
    bool operator<=(VALUE const& _rhs) const { return m_data <= _rhs.asBigInt(); }
    bool operator!=(VALUE const& _rhs) const { return m_data != _rhs.asBigInt(); }
    bool operator==(VALUE const& _rhs) const { return m_data == _rhs.asBigInt(); }

    VALUE operator-(VALUE const& _rhs) const { return VALUE(m_data - _rhs.asBigInt()); }
    VALUE operator-(long long  _rhs) const { return VALUE(m_data - _rhs); }
    VALUE operator/(VALUE const& _rhs) const { return VALUE(m_data / _rhs.asBigInt()); }
    VALUE operator/(long long  _rhs) const { return VALUE(m_data / _rhs); }
    VALUE operator*(VALUE const& _rhs) const { return VALUE(m_data * _rhs.asBigInt()); }
    VALUE operator*(long long  _rhs) const { return VALUE(m_data * _rhs); }
    VALUE operator+(VALUE const& _rhs) const { return VALUE(m_data + _rhs.asBigInt()); }
    VALUE operator+(long long  _rhs) const { return VALUE(m_data + _rhs); }

    VALUE& operator+=(VALUE const& _rhs) { m_data += _rhs.asBigInt(); m_dirty = true; return *this; }
    VALUE& operator+=(long long  _rhs) { m_data += _rhs; m_dirty = true; return *this; }
    VALUE& operator-=(VALUE const& _rhs) { m_data -= _rhs.asBigInt(); m_dirty = true; return *this; }
    VALUE& operator-=(long long  _rhs) { m_data -= _rhs; m_dirty = true; return *this; }
    VALUE& operator/=(VALUE const& _rhs) { m_data /= _rhs.asBigInt(); m_dirty = true; return *this; }
    VALUE& operator/=(long long  _rhs) { m_data /= _rhs; m_dirty = true; return *this; }
    VALUE& operator*=(VALUE const& _rhs) { m_data *= _rhs.asBigInt(); m_dirty = true; return *this; }
    VALUE& operator*=(long long  _rhs) { m_data *= _rhs; m_dirty = true; return *this; }

    VALUE operator++(int) { m_data++; return *this; }

    std::string const& asString() const;
    std::string asDecString() const;
    dev::bigint const& asBigInt() const { return m_data; }
    dev::bytes const& serializeRLP() const;
    bool isBigInt() const { return m_bigint; }

private:
    VALUE() {}
    std::string verifyHexString(std::string const& _s, std::string const& _k = std::string()) const;
    void calculateCache() const;
    dev::bigint m_data;

    // Optimizations
    mutable bool m_dirty = true;
    mutable std::string m_dataStrZeroXCache;
    mutable dev::bytes m_bytesData;

    // Bigint specific
    bool m_bigint = false;
    mutable bool m_bigintEmpty = false;
    mutable dev::bytes m_bytesBigIntData;
    mutable size_t m_prefixedZeros = 0;
    mutable std::string m_dataStrBigIntCache;
};

typedef GCP_SPointer<VALUE> spVALUE;


}  // namespace teststruct
