#include "lists.h"
#include "../ssz.h"
#include <stdexcept>
using namespace ssz::encoding;

namespace ssz
{
SSZStream& SSZStream::operator<<(BitList const& _value)
{
    const size_t numBits = _value.size();
    const size_t numBytes = (numBits + 7) / BITS_PER_BYTE;
    const size_t offset = m_out.size();
    m_out.resize(m_out.size() + numBytes);

    size_t i = 0;
    for (auto const& el : _value)
    {
        if (el)
            m_out[offset + i / BITS_PER_BYTE] |= (1 << (i % BITS_PER_BYTE));
        i++;
    }
    auto const lastElement = offset + numBits / BITS_PER_BYTE;
    if (lastElement >= m_out.size())
        m_out.resize(m_out.size() + 1);
    m_out[lastElement] |= (1 << (numBits % BITS_PER_BYTE));
    return *this;
}

namespace encoding
{
BitList decodeBitList(bytes const& _input, size_t _size)
{
    const size_t numBytes = _input.size();
    const size_t numBits = numBytes * BITS_PER_BYTE;

    BitList result;

    // Iterate through the input bytes and populate the BitList
    for (size_t i = 0; i < numBits; i++)
    {
        const size_t byteIndex = i / BITS_PER_BYTE;
        const size_t bitPosition = i % BITS_PER_BYTE;

        // Check if the bit is set in the input bytes
        const bool bitSet = _input[byteIndex] & (1 << bitPosition);
        if (result.size() >= _size)
        {
            // Check last bit
            if (bitSet != true)
                throw std::invalid_argument("decodeBitList expected a finalizing list bit `1` at the end!");
            // check rest bits are 0 and no more bytes follow
            return result;
        }

        if (result.size() < _size)
            result.push_back(bitSet);
    }

    throw std::invalid_argument("decodeBitList expected a finalizing list bit `1` at the end, but the input finished!");
    return result;
}


}  // namespace encoding
}  // namespace ssz
