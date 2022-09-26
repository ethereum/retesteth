#pragma once
#include <libdataobj/DataObject.h>
#include <retesteth/testStructures/types/Ethereum/EthereumBlock.h>

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
spDataObject constructEthGetBlockBy(EthereumBlockState const& _block);

// RLP Validators
void verifyBlockRLP(dev::RLP const& _rlp);

// Prepare data for ToolChainManager::test_calculateDifficulty
struct DifficultyStatic
{
    spBlockHeader blockA;
    FH32 loghash = FH32("0x1122334455667788991011121314151617181920212223242526272829303132");
    spState state;
};
DifficultyStatic const& prepareEthereumBlockStateTemplate();

}  // namespace toolimpl
