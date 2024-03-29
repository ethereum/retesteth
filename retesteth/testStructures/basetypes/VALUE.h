#pragma once
#include <libdataobj/DataObject.h>
#include <libdevcore/Common.h>
#include <libdevcore/RLP.h>

namespace test::teststruct
{

// Validate and manage the type of VALUE (bigInt)
// Deserialized from string of "0x1122...32", "123343"
// Can be limited by _limit max value

struct VALUE : dataobject::GCP_SPointerBase
{
    // TODO ideally separate bigint logic into another class that behave exactly the same as VALUE but with exceptions
    explicit VALUE(dev::RLP const& _rlp);
    VALUE(dev::bigint const&);
    VALUE(int);
    explicit VALUE(dataobject::DataObject const&);  // Does not require to move smart pointer here as this structure changes a lot
    explicit VALUE(std::string const&);
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
    void _fromString(std::string const& _data, std::string const& _hintkey = std::string());
    std::string verifyHexString(std::string const& _s, std::string const& _k = std::string()) const;
    void calculateCache() const;
    size_t _countPrefixedBytes(std::string const&) const;
    dev::bigint m_data;

    // Optimizations
    mutable bool m_dirty = true;
    mutable std::string m_dataStr;
    mutable dev::bytes m_bytesData;

    // Bigint specific
    bool m_bigint = false;
    mutable bool m_bigintEmpty = false;
    mutable size_t m_prefixedZeroBytes = 0;
};

typedef dataobject::GCP_SPointer<VALUE> spVALUE;
template <class T>
spVALUE sVALUE(T const& _arg) { return spVALUE(new VALUE(_arg)); }


}  // namespace teststruct
