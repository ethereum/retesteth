#include "ToolImplHelper.h"
#include <retesteth/TestHelper.h>
#include <retesteth/testStructures/Common.h>
using namespace test;
using namespace test::debug;
using namespace std;

namespace  {
    mutex g_DifficultyStatic_Access;
}

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

spDataObject constructEthGetBlockBy(EthereumBlockState const& _block)
{
    spDataObject constructResponse = _block.header()->asDataObject();

    spDataObject transactionArray(new DataObject(DataType::Array));
    (*constructResponse).atKeyPointer("transactions") = transactionArray;
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

    spDataObject arr(new DataObject(DataType::Array));
    (*constructResponse).atKeyPointer("uncles") = arr;
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

// Prepare data for ToolChainManager::test_calculateDifficulty
DifficultyStatic const& prepareEthereumBlockStateTemplate()
{
    std::lock_guard<std::mutex> lock(g_DifficultyStatic_Access);
    static DifficultyStatic data;
    if (data.blockA.isEmpty())
    {
        spDataObject accountData;
        (*accountData).setKey("0x1122334455667788991011121314151617181920");
        (*accountData)["balance"] = "0x1000";
        (*accountData)["code"] = "0x";
        (*accountData)["nonce"] = "0x1000";
        (*accountData).atKeyPointer("storage") = spDataObject(new DataObject(DataType::Object));
        std::map<FH20, spAccountBase> state = {
            {FH20("0x1122334455667788991011121314151617181920"), spAccountBase(new State::Account(accountData))}};

        DataObject headerAData;
        headerAData["bloom"] =
            "0x0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
            "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
            "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
            "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
            "0000000000000000000000000000000000000000000000000000000000";
        headerAData["coinbase"] = "0x2adc25665018aa1fe0e6bc666dac8fc2697ff9ba";
        headerAData["extraData"] = "0x00";
        headerAData["gasLimit"] = "0x0f4240";
        headerAData["gasUsed"] = "0x5d70";
        headerAData["hash"] = "0x668c5d6c12a905b58fc597b3146d29b9d12280c1891449b06e19f9edee0fe86f";
        headerAData["mixHash"] = "0x0000000000000000000000000000000000000000000000000000000000000000";
        headerAData["nonce"] = "0x0000000000000000";
        headerAData["parentHash"] = "0xd86c2d4e6439870452a6c52b71fc6f06d9f98e53c5f9c47acd0ab3c04e73ef7b";
        headerAData["receiptTrie"] = "0xb34b65874cf7cb8358930db57604b2f6610f98c25f2b5822cc25adee7f181ff5";
        headerAData["stateRoot"] = "0xf17157ed407eb909de943044b712ab5db7f48c3557e3a3d8e17b1d8b426a1916";
        headerAData["transactionsTrie"] = "0x174ccc4f3050aca8eb96ee492d7f77c48698de3988b3a422e2bc6974348182c0";
        headerAData["uncleHash"] = "0x1dcc4de8dec75d7aab85b567b6ccd41ad312451b948a7413f0a142fd40d49347";
        headerAData["difficulty"] = "0x131415";
        headerAData["number"] = "0x01";
        headerAData["timestamp"] = "0x00";
        headerAData["baseFeePerGas"] = "0x10";

        data.blockA = spBlockHeader(new BlockHeader1559(headerAData));
        data.state = spState(new State(state));
    }
    return data;
}

}  // namespace toolimpl
