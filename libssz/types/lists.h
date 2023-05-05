#pragma once
#include "basic.h"

namespace ssz
{
using BitList = std::list<bit>;

namespace encoding
{
BitList decodeBitList(bytes const& _input, size_t _size);
}
}  // namespace ssz
