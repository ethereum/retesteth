#pragma once
#include <retesteth/dataObject/DataObject.h>
#include <retesteth/testStructures/types/Ethereum/EthereumBlock.h>
using namespace dataobject;

namespace toolimpl
{
// Construct accountRange RPC style
DataObject constructAccountRange(EthereumBlockState const& _block, FH32 const& _addrHash, size_t _maxResult);

// Construct storageRange RPC style
DataObject constructStorageRangeAt(
    EthereumBlockState const& _block, FH20 const& _address, FH32 const& _begin, size_t _maxResult);

// Construct RPC style response
DataObject constructEthGetBlockBy(EthereumBlockState const& _block);

// RLP Validators
void verifyBlockRLP(dev::RLP const& _rlp);

}  // namespace toolimpl


/*
using namespace dataobject;
namespace toolimpl
{
// Reading dev::RLP into string
std::string rlpToString(dev::RLP const&);

// Sanitize ethereum structures / objects from RLP
class BlockHeadFromRLP
{
public:
    BlockHeadFromRLP(dev::RLP const&);
    scheme_RPCBlock getRPCResponse() const;

private:
    class validator
    {
    public:
        validator(dev::RLP const&);
    };
    validator m_validator;
    DataObject rlpToData(dev::RLP const&) const;

public:
    scheme_blockHeader header;
};

class TransactionFromRLP
{
public:
    TransactionFromRLP(dev::RLP const&);

private:
    class validator
    {
    public:
        validator(dev::RLP const&);
    };
    validator m_validator;
    DataObject rlpToData(dev::RLP const&);

public:
    scheme_transaction transaction;
};

}  // namespace toolimpl

*/
