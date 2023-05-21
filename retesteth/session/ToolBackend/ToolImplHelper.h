#pragma once
#include <libdataobj/DataObject.h>
#include <retesteth/testStructures/types/Ethereum/EthereumBlock.h>
#include <retesteth/session/SessionInterface.h>

namespace toolimpl
{
using namespace dataobject;
using namespace test::teststruct;

// Construct accountRange RPC style
spDataObject constructAccountRange(EthereumBlockState const& _block, FH32 const& _addrHash, size_t _maxResult);

// Construct storageRange RPC style
spDataObject constructStorageRangeAt(
    EthereumBlockState const& _block, FH20 const& _address, FH32 const& _begin, size_t _maxResult);

// Construct RPC style response
spDataObject constructEthGetBlockBy(EthereumBlockState const& _block, test::session::Request);

// RLP Validators
void verifyBlockRLP(dev::RLP const& _rlp);

}  // namespace toolimpl
