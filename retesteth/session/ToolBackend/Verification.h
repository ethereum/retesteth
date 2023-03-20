#pragma once
#include "ToolChainHelper.h"
#include <retesteth/testStructures/types/Ethereum/Blocks/BlockHeader.h>

namespace toolimpl
{
const size_t BASE_FEE_MAX_CHANGE_DENOMINATOR = 8;
const size_t ELASTICITY_MULTIPLIER = 2;
const size_t INITIAL_BASE_FEE = 1000000000;
const size_t MAX_EXTRADATA_SIZE_IN_MERGE = 32;
const VALUE POW2_64(DataObject("0x10000000000000000"));

// Blockchain logic validator
void verifyEthereumBlockHeader(spBlockHeader const& _header, ToolChain const& _chain);

// Verify Withdrawals RLP in body
void verifyWithdrawalsRLP(dev::RLP const& _rlp);
void verifyWithdrawalRecord(spWithdrawal const& _wtRecord);

}  // namespace toolimpl
