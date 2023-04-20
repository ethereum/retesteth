#pragma once
#if defined(UNITTESTS) || defined(__DEBUG__)

#include <vector>
#include <stdint.h>
#include <boost/multiprecision/cpp_int.hpp>

namespace ssz {

using bit = bool;
using BitVector = std::vector<bit>;
using byte = uint8_t;
using bytes = std::vector<byte>;
using u128 =  boost::multiprecision::number<boost::multiprecision::cpp_int_backend<128, 128, boost::multiprecision::unsigned_magnitude, boost::multiprecision::unchecked, void>>;
using u256 =  boost::multiprecision::number<boost::multiprecision::cpp_int_backend<256, 256, boost::multiprecision::unsigned_magnitude, boost::multiprecision::unchecked, void>>;

const short BITS_PER_BYTE = 8;

template<typename T>
concept Integral = std::is_integral_v<T>;

class SSZStream
{
public:

    template <Integral T>
    SSZStream& operator<<(T const& _value) { addIntegral(_value); return *this; }
    SSZStream& operator<<(ssz::u128 const& _value) { addIntegral(_value, 16); return *this; }
    SSZStream& operator<<(ssz::u256 const& _value) { addIntegral(_value, 32); return *this; }
    SSZStream& operator<<(BitVector const& _value);

    bytes const& data() const { return m_out; }

private:
    template <typename T>
    void addIntegral(const T& _value, size_t _size = 0);
private:
    bytes m_out;
};

}
#endif
