#include "ToolImplHelper.h"
#include <retesteth/TestHelper.h>
#include <retesteth/testStructures/Common.h>
using namespace test;

namespace toolimpl
{
DataObject constructAccountRange(EthereumBlockState const& _block, FH32 const& _addrHash, size_t _maxResult)
{
    size_t k = 0;
    size_t iAcc = hexOrDecStringToInt(_addrHash.asString());
    DataObject constructResponse;
    constructResponse["addressMap"] = DataObject(DataType::Object);
    for (auto const& acc : _block.state().accounts())
    {
        if (k++ + 1 < iAcc)  // The first key is 1 must be included
            continue;

        constructResponse["addressMap"].addSubObject(fto_string(iAcc++), DataObject(acc.first.asString()));
        if (constructResponse.atKey("addressMap").getSubObjects().size() == _maxResult)
            break;
    }
    if (constructResponse.atKey("addressMap").getSubObjects().size() == 0)
        constructResponse["nextKey"] = test::stoCompactHexPrefixed(0, 32);
    else
        constructResponse["nextKey"] = test::stoCompactHexPrefixed(k + 1, 32);  // because we rely on empty structure +1
    return constructResponse;
}

DataObject constructEthGetBlockBy(EthereumBlockState const& _block)
{
    DataObject constructResponse = _block.header().getCContent().asDataObject();

    constructResponse["transactions"] = DataObject(DataType::Array);
    for (auto const& tr : _block.transactions())
    {
        DataObject fullTransaction = tr.getCContent().asDataObject();
        fullTransaction["blockHash"] =
            _block.header().getCContent().hash().asString();  // We don't know the hash its in tool response
        fullTransaction["blockNumber"] = _block.header().getCContent().number().asString();
        fullTransaction["from"] = FH20::zero().asString();  // Can be recovered from vrs
        fullTransaction["transactionIndex"] = "0x00";       // Its in tool response
        fullTransaction["hash"] = tr.getCContent().hash().asString();
        constructResponse["transactions"].addArrayObject(fullTransaction);
    }

    constructResponse["uncles"] = DataObject(DataType::Array);
    for (auto const& un : _block.uncles())
        constructResponse["uncles"].addArrayObject(un.getCContent().hash().asString());

    constructResponse["size"] = "0x00";
    constructResponse["totalDifficulty"] = "0x00";
    constructResponse.renameKey("bloom", "logsBloom");
    constructResponse.renameKey("coinbase", "miner");
    constructResponse.renameKey("receiptTrie", "receiptsRoot");
    constructResponse.renameKey("transactionsTrie", "transactionsRoot");
    constructResponse.renameKey("uncleHash", "sha3Uncles");

    return constructResponse;
}

DataObject constructStorageRangeAt(
    EthereumBlockState const& _block, FH20 const& _address, FH32 const& _begin, size_t _maxResult)
{
    DataObject constructResponse;
    if (_block.state().hasAccount(_address))
    {
        constructResponse["complete"].setBool(true);
        constructResponse["storage"] = DataObject(DataType::Object);
        constructResponse["nextKey"] = FH32::zero().asString();
        if (_block.state().getAccount(_address).hasStorage())
        {
            size_t iStore = 0;
            size_t iBegin = hexOrDecStringToInt(_begin.asString());
            for (auto const& el : _block.state().getAccount(_address).storage().getKeys())
            {
                if (iStore++ + 1 < iBegin)
                    continue;
                DataObject record;
                record["key"] = std::get<0>(el.second).getCContent().asString();
                record["value"] = std::get<1>(el.second).getCContent().asString();
                record.performModifier(mod_removeLeadingZerosFromHexValuesEVEN);
                constructResponse["storage"][fto_string(iStore)] = record;
                if (constructResponse.atKey("storage").getSubObjects().size() == _maxResult)
                {
                    constructResponse["complete"].setBool(false);
                    constructResponse["nextKey"] = dev::toCompactHexPrefixed(iStore + 1, 32);
                    break;
                }
            }
        }
        else
            constructResponse["storage"] = DataObject();
    }
    ETH_LOG(constructResponse.asJson(), 7);
    return constructResponse;
}

// RLP Validators
void verifyBlockRLP(dev::RLP const& _rlp)
{
    if (!_rlp.isList())
        throw dev::RLPException("RLP is expected to be list");

    if (!_rlp[0].isList())
        throw dev::RLPException("BlockHeader RLP is expected to be list");

    for (size_t i = 0; i < 15; i++)
    {
        if (!_rlp[0][i].isData())
            throw dev::RLPException("Blockheader RLP field is not data!");
    }

    for (auto const& tr : _rlp[1])
    {
        if (tr.isList())
        {
            // check legacy transaction. otherwise accept byte array
            for (size_t i = 0; i < 9; i++)
            {
                if (!tr[i].isData())
                    throw dev::RLPException("Transaction RLP field is not data!");
            }
        }
        else if (tr.isData())
        {
            // Transaction type 1 is allowed
            if ((int)tr.payload()[0] != 1 && (int)tr.payload()[0] != 2)
                throw dev::RLPException("Transaction RLP is expected to be list");
        }
        else
            throw dev::RLPException("Transaction RLP is expected to be list");
    }

    for (auto const& un : _rlp[2])
    {
        if (!un.isList())
            throw dev::RLPException("Uncleheader RLP is expected to be list");

        for (size_t i = 0; i < 15; i++)
        {
            if (!un[i].isData())
                throw dev::RLPException("Uncleheader RLP field is not data!");
        }
    }
}
}  // namespace toolimpl
