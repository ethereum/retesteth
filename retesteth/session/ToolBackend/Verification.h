#pragma once
#include "ToolChainHelper.h"
#include <retesteth/testStructures/Common.h>

namespace toolimpl
{
const size_t BASE_FEE_MAX_CHANGE_DENOMINATOR = 8;
const size_t ELASTICITY_MULTIPLIER = 2;

// Blockchain logic validator
void verifyEthereumBlockHeader(spBlockHeader const& _header, ToolChain const& _chain);

}  // namespace toolimpl
