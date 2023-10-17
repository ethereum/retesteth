#include "vectors.h"
#include "../ssz.h"
using namespace ssz::encoding;

namespace ssz
{
SSZStream& SSZStream::operator<<(BitVector const& _value)
{
    const size_t numBits = _value.size();
    const size_t numBytes = (numBits + 7) / BITS_PER_BYTE;
    const size_t offset = m_out.size();
    m_out.resize(m_out.size() + numBytes);

    for (size_t i = 0; i < numBits; i++)
    {
        if (_value[i])
            m_out[offset + i / BITS_PER_BYTE] |= (1 << (i % BITS_PER_BYTE));
    }
    return *this;
}


namespace encoding
{
BitVector decodeBitVector(bytes const& _input, size_t _size)
{
    const size_t numBytes = _input.size();
    const size_t numBits = numBytes * BITS_PER_BYTE;  // Assuming 8 bits per byte

    // Create a new BitVector object
    BitVector result;

    // Iterate through the input bytes and populate the BitVector
    for (size_t i = 0; i < numBits; i++)
    {
        const size_t byteIndex = i / BITS_PER_BYTE;
        const size_t bitPosition = i % BITS_PER_BYTE;

        // Check if the bit is set in the input bytes
        const bool bitSet = _input[byteIndex] & (1 << bitPosition);

        // Add the bit to the BitVector
        if (result.size() < _size)
            result.push_back(bitSet);
        else
            return result;
    }
    return result;
}
}  // namespace encoding
}  // namespace ssz
