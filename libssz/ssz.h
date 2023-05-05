#pragma once
#include "libdataobj/DataObject.h"
#include "types/lists.h"
#include "types/uints.h"
#include "types/vectors.h"

namespace ssz
{
class SSZStream
{
public:
    template <encoding::Integral T>
    SSZStream& operator<<(T const& _value)
    {
        m_out = encoding::integralToBytes(_value);
        return *this;
    }
    SSZStream& operator<<(uint const& _value)
    {
        *this << _value.data;
        return *this;
    }
    SSZStream& operator<<(BitVector const& _value);
    SSZStream& operator<<(bytes const& _value);
    SSZStream& operator<<(BitList const& _value);

    bytes const& data() const { return m_out; }

private:
    bytes m_out;
};

class SSZ
{
public:
    SSZ(bytes const& _data, dataobject::DataObject const& _scheme);
    dataobject::DataObject const& data() const { return m_out; }

private:
    dataobject::DataObject m_out;
};

ssz::SSZType StringToSSZType(std::string const& _type, size_t& _size);

}  // namespace ssz
