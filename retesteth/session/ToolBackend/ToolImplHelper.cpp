#include "ToolImplHelper.h"
#include <retesteth/helpers/TestHelper.h>
#include <retesteth/testStructures/Common.h>
using namespace test;
using namespace test::debug;
using namespace std;

namespace toolimpl
{
spDataObject constructAccountRange(EthereumBlockState const& _block, FH32 const& _addrHash, size_t _maxResult)
{
    size_t k = 0;
    size_t iAcc = hexOrDecStringToInt(_addrHash.asString());
    spDataObject constructResponse;
    spDataObject emptyList(new DataObject(DataType::Object));
    (*constructResponse).atKeyPointer("addressMap") = emptyList;
    for (auto const& acc : _block.state()->accounts())
    {
        if (k++ + 1 < iAcc)  // The first key is 1 must be included
            continue;

        spDataObject address(new DataObject(acc.first.asString()));
        (*constructResponse)["addressMap"].addSubObject(fto_string(iAcc++), address);
        if (constructResponse->atKey("addressMap").getSubObjects().size() == _maxResult)
            break;
    }
    if (constructResponse->atKey("addressMap").getSubObjects().size() == 0)
        (*constructResponse)["nextKey"] = test::stoCompactHexPrefixed(0, 32);
    else
        (*constructResponse)["nextKey"] = test::stoCompactHexPrefixed(k + 1, 32);  // because we rely on empty structure +1
    return constructResponse;
}

spDataObject constructEthGetBlockBy(EthereumBlockState const& _block, test::session::Request _request)
{
    // Imitate eth_getBlockBy...  rpc response
    spDataObject constructResponse = _block.header()->asDataObject();

    spDataObject transactionArray(new DataObject(DataType::Array));
    (*constructResponse).atKeyPointer("transactions") = transactionArray;
    spDataObject arr(new DataObject(DataType::Array));
    (*constructResponse).atKeyPointer("uncles") = arr;

    if (_request == test::session::Request::FULLOBJECTS)
    {
        for (auto const& tr : _block.transactions())
        {
            spDataObject fullTransaction = tr->asDataObject();
            (*fullTransaction)["blockHash"] = _block.header()->hash().asString();  // We don't know the hash its in tool response
            (*fullTransaction)["blockNumber"] = _block.header()->number().asString();
            (*fullTransaction)["from"] = FH20::zero().asString();  // Can be recovered from vrs
            (*fullTransaction)["transactionIndex"] = "0x00";       // Its in tool response
            (*fullTransaction)["hash"] = tr->hash().asString();
            (*constructResponse)["transactions"].addArrayObject(fullTransaction);
        }

        for (auto const& wt : _block.withdrawals())
        {
            (*constructResponse)["withdrawals"].addArrayObject(wt->asDataObject());
        }
    }
    else
    {
        for (auto const& tr : _block.transactions())
        {
            spDataObject trHash(new DataObject(tr->hash().asString()));
            (*constructResponse)["transactions"].addArrayObject(trHash);
        }
    }

    for (auto const& un : _block.uncles())
    {
        spDataObject unHash(new DataObject(un->hash().asString()));
        (*constructResponse)["uncles"].addArrayObject(unHash);
    }

    (*constructResponse)["size"] = "0x00";
    (*constructResponse)["totalDifficulty"] = "0x00";
    (*constructResponse).renameKey("bloom", "logsBloom");
    (*constructResponse).renameKey("coinbase", "miner");
    (*constructResponse).renameKey("receiptTrie", "receiptsRoot");
    (*constructResponse).renameKey("transactionsTrie", "transactionsRoot");
    (*constructResponse).renameKey("uncleHash", "sha3Uncles");
    return constructResponse;
}

spDataObject constructStorageRangeAt(
    EthereumBlockState const& _block, FH20 const& _address, FH32 const& _begin, size_t _maxResult)
{
    spDataObject constructResponse;
    if (_block.state()->hasAccount(_address))
    {
        (*constructResponse)["complete"].setBool(true);
        spDataObject obj(new DataObject(DataType::Object));
        (*constructResponse).atKeyPointer("storage") = obj;
        (*constructResponse)["nextKey"] = FH32::zero().asString();
        if (_block.state()->getAccount(_address).hasStorage())
        {
            size_t iStore = 0;
            size_t iBegin = hexOrDecStringToInt(_begin.asString());
            for (auto const& el : _block.state()->getAccount(_address).storage().getKeys())
            {
                if (iStore++ + 1 < iBegin)
                    continue;
                DataObject& record = (*obj)[fto_string(iStore)];
                record["key"] = std::get<0>(el.second)->asString();
                record["value"] = std::get<1>(el.second)->asString();
                record.performModifier(mod_removeLeadingZerosFromHexValueEVEN);

                if (constructResponse->atKey("storage").getSubObjects().size() == _maxResult)
                {
                    (*constructResponse)["complete"].setBool(false);
                    (*constructResponse)["nextKey"] = dev::toCompactHexPrefixed(iStore + 1, 32);
                    break;
                }
            }
        }
        else
        {
            (*constructResponse)["storage"];
        }
    }
    ETH_DC_MESSAGE(DC::LOWLOG, constructResponse->asJson());
    return constructResponse;
}

// RLP Validators
void verifyBlockRLP(dev::RLP const& _rlp)
{
    size_t const blockheader = 0;
    size_t const transactions = 1;
    size_t const uncles = 2;
    if (!_rlp.isList())
        throw dev::RLPException("RLP is expected to be list");

    if (!_rlp[blockheader].isList())
        throw dev::RLPException("BlockHeader RLP is expected to be list");

    for (size_t i = 0; i < 15; i++)
    {
        if (!_rlp[blockheader][i].isData())
            throw dev::RLPException("Blockheader RLP field is not data!");
    }

    for (auto const& tr : _rlp[transactions])
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
            const size_t trType = (size_t)tr.payload()[0];
            if (trType > 3)
                throw dev::RLPException("Transaction RLP is expected to be list (Type not supported)");
        }
        else
            throw dev::RLPException("Transaction RLP is expected to be list");
    }

    for (auto const& un : _rlp[uncles])
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
