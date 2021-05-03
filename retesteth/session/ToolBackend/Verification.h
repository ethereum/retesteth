#pragma once
#include "ToolChainHelper.h"
#include <retesteth/testStructures/Common.h>

namespace toolimpl
{
// Blockchain logic validator
void verifyEthereumBlockHeader(spBlockHeader const& _header, ToolChain const& _chain);

}  // namespace toolimpl
