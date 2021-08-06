#pragma once
#include <retesteth/dataObject/DataObject.h>
#include <retesteth/testStructures/types/Ethereum/EthereumBlock.h>
using namespace dataobject;

namespace toolimpl
{
// Construct accountRange RPC style
spDataObject constructAccountRange(EthereumBlockState const& _block, FH32 const& _addrHash, size_t _maxResult);

// Construct storageRange RPC style
spDataObject constructStorageRangeAt(
    EthereumBlockState const& _block, FH20 const& _address, FH32 const& _begin, size_t _maxResult);

// Construct RPC style response
spDataObject constructEthGetBlockBy(EthereumBlockState const& _block);

// RLP Validators
void verifyBlockRLP(dev::RLP const& _rlp);

}  // namespace toolimpl
