#pragma once
#include "basic.h"

namespace ssz
{
using BitVector = std::vector<bit>;

namespace encoding
{
BitVector decodeBitVector(bytes const& _input, size_t _size);
}
}  // namespace ssz
