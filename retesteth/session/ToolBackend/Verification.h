#pragma once
#include "ToolChainHelper.h"
#include <retesteth/testStructures/types/Ethereum/Blocks/BlockHeader.h>

namespace toolimpl
{
const size_t BASE_FEE_MAX_CHANGE_DENOMINATOR = 8;
const size_t ELASTICITY_MULTIPLIER = 2;
const size_t INITIAL_BASE_FEE = 1000000000;
const size_t MAX_EXTRADATA_SIZE_IN_MERGE = 32;
const size_t MIN_BLOB_GASPRICE = 1;
const size_t BLOB_GASPRICE_UPDATE_FRACTION = 3338477;
const VALUE POW2_64(DataObject("0x010000000000000000"));

// Blockchain logic validator
void verifyEthereumBlockHeader(spBlockHeader const& _header, ToolChain const& _chain);

// Verify Withdrawals RLP in body
void verifyWithdrawalsRLP(dev::RLP const& _rlp);
void verifyWithdrawalRecord(spWithdrawal const& _wtRecord);
int get_blob_gasprice(BlockHeader4844 const& _header);


}  // namespace toolimpl
