#pragma once
#include <stdint.h>
#include <array>
#include <list>
#include <string>
#include <vector>

namespace ssz
{
enum SSZType
{
    UNDEFINED,
    Null,
    Bool,
    Uint8,
    Uint16,
    Uint32,
    Uint64,
    Uint128,
    Uint256,
    Bitlist,
    Bitvector
};

using bit = bool;
using byte = uint8_t;
using bytes = std::vector<byte>;
using Bytes32 = std::array<byte, 32>;

const short BITS_PER_BYTE = 8;

bytes stringToBytes(std::string const& _input);
std::string bytesToString(bytes const& _input);

}  // namespace ssz


// DEBUG
#include "iostream"
#include <iomanip>
void print_bytes(const ssz::bytes& data);
// DEBUG
