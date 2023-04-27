#include "ssz.h"
#if defined(UNITTESTS) || defined(__DEBUG__)
namespace ssz {

SSZStream& SSZStream::operator<<(BitVector const& _value)
{
    const size_t numBits = _value.size();
    const size_t numBytes = (numBits + 7) / BITS_PER_BYTE;
    const size_t offset = m_out.size();
    m_out.resize(m_out.size() + numBytes);

    for (size_t i = 0; i < numBits; i++) {
        if (_value[i]) {
            m_out[offset + i / BITS_PER_BYTE] |= (1 << (i % BITS_PER_BYTE));
        }
    }
    //m_out[offset + numBits / BITS_PER_BYTE] |= (1 << (numBits % BITS_PER_BYTE));
    return *this;
}

template <typename T>
void SSZStream::addIntegral(const T& _value, size_t _size)
{
    const size_t size = _size == 0 ? sizeof(T) : _size;
    for (size_t i = 0; i < size; ++i) {
        m_out.emplace_back((_value >> (BITS_PER_BYTE * i)) & 0xFF);
    }
}
template void SSZStream::addIntegral<bool>(bool const&, size_t);
template void SSZStream::addIntegral<uint8_t>(uint8_t const&, size_t);
template void SSZStream::addIntegral<uint16_t>(uint16_t const&, size_t);
template void SSZStream::addIntegral<uint32_t>(uint32_t const&, size_t);
template void SSZStream::addIntegral<uint64_t>(uint64_t const&, size_t);
template void SSZStream::addIntegral<ssz::u128>(ssz::u128 const&, size_t);
template void SSZStream::addIntegral<ssz::u256>(ssz::u256 const&, size_t);

}
#endif
