#pragma once
#include "basic.h"

namespace ssz
{
struct uint
{
    bytes data;

protected:
    uint(uint64_t _data, size_t _size);
    uint(std::string const& _data, size_t _size);
    const size_t m_size;
};

struct uint8 : public uint
{
    uint8(uint64_t _data) : uint(_data, 1) {}
    uint8(std::string const& _data) : uint(_data, 1) {}
};

struct uint16 : public uint
{
    uint16(uint64_t _data) : uint(_data, 2) {}
    uint16(std::string const& _data) : uint(_data, 2) {}
};

struct uint32 : public uint
{
    uint32(uint64_t _data) : uint(_data, 4) {}
    uint32(std::string const& _data) : uint(_data, 4) {}
};

struct uint64 : public uint
{
    uint64(uint64_t _data) : uint(_data, 8) {}
    uint64(std::string const& _data) : uint(_data, 8) {}
};

struct uint128 : public uint
{
    uint128(std::string const& _data) : uint(_data, 16) {}
};

struct uint256 : public uint
{
    uint256(std::string const& _data) : uint(_data, 32) {}
};

namespace encoding
{
template <typename T>
concept Integral = std::is_integral_v<T> && !std::is_same_v<T, ssz::uint>;

template <Integral T>
bytes integralToBytes(const T& _value, size_t _size = 0);

template <typename T, typename std::enable_if<std::is_integral<T>::value, int>::type = 0>
T bytesToIntegral(const bytes& _bytes, size_t _size = 0);

}  // namespace encoding
}  // namespace ssz
