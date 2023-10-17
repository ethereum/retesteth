#include "uints.h"
#include <cmath>
#include <stdexcept>
using namespace std;
using namespace ssz;
using namespace ssz::encoding;

namespace ssz
{
ssz::uint::uint(std::string const& _data, size_t _size) : m_size(_size)
{
    data = stringToBytes(_data);
    if (data.size() > _size)
        throw std::invalid_argument("Input string is too long for a " + to_string(_size * 8) +
                                    "-bit unsigned integer \n"
                                    "Got bytes: " +
                                    to_string(data.size()) + " from dec (" + _data + ")");
    for (size_t i = data.size(); i < _size; i++)
        data.push_back((byte)0);
}

ssz::uint::uint(uint64_t _data, size_t _size) : m_size(_size)
{
    if (_size > 8)
        throw std::invalid_argument("uint(uint64_t _data, size_t _size) got _size > 8");
    const uint64_t max = pow(2, 8 * _size) - 1;
    if (_data > max)
        throw std::invalid_argument("Input string is too long for a " + to_string(_size * 8) +
                                    "-bit unsigned integer \n"
                                    "Converting from dec (" +
                                    to_string(_data) + ")");
    data = integralToBytes(_data, m_size);
}


namespace encoding
{
template <Integral T>
bytes integralToBytes(const T& _value, size_t _size)
{
    bytes out;
    const size_t size = _size == 0 ? sizeof(T) : _size;
    for (size_t i = 0; i < size; ++i)
        out.emplace_back((_value >> (BITS_PER_BYTE * i)) & 0xFF);
    return out;
}
template bytes integralToBytes<bool>(bool const&, size_t);
template bytes integralToBytes<uint8_t>(uint8_t const&, size_t);
template bytes integralToBytes<uint16_t>(uint16_t const&, size_t);
template bytes integralToBytes<uint32_t>(uint32_t const&, size_t);
template bytes integralToBytes<uint64_t>(uint64_t const&, size_t);

template <typename T, typename std::enable_if<std::is_integral<T>::value, int>::type>
T bytesToIntegral(const bytes& _bytes, size_t _size)
{
    T value = 0;
    const size_t size = _size == 0 ? sizeof(T) : _size;
    for (size_t i = 0; i < size; ++i)
        value |= static_cast<T>(_bytes[i]) << (BITS_PER_BYTE * i);
    return value;
}
template uint8_t bytesToIntegral<uint8_t>(bytes const&, size_t);
template uint16_t bytesToIntegral<uint16_t>(bytes const&, size_t);

}  // namespace encoding
}  // namespace ssz
