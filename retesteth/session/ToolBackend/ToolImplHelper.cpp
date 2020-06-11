#include "ToolImplHelper.h"
#include <retesteth/TestHelper.h>
#include <retesteth/session/ToolImpl.h>
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
    DataObject constructResponse = _block.header().asDataObject();

    constructResponse["transactions"] = DataObject(DataType::Array);
    for (auto const& tr : _block.transactions())
    {
        DataObject fullTransaction = tr.asDataObject();
        fullTransaction["blockHash"] = _block.header().hash().asString();  // We don't know the hash its in tool response
        fullTransaction["blockNumber"] = _block.header().number().asString();
        fullTransaction["from"] = FH20::zero().asString();  // Can be recovered from vrs
        fullTransaction["transactionIndex"] = "0x00";       // Its in tool response
        fullTransaction["hash"] = tr.hash().asString();
        constructResponse["transactions"].addArrayObject(fullTransaction);
    }

    constructResponse["uncles"] = DataObject(DataType::Array);
    for (auto const& un : _block.uncles())
        constructResponse["uncles"].addArrayObject(un.hash().asString());

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

}  // namespace toolimpl
/*
namespace toolimpl
{
string rlpToString(dev::RLP const& _rlp)
{
    std::ostringstream stream;
    stream << _rlp.toBytes();
    if (stream.str() == "0x")
        return "0x0";
    return stream.str();
}

// Block Header Sanitizer when parsing RLP
BlockHeadFromRLP::BlockHeadFromRLP(dev::RLP const& _rlp)
  : m_validator(_rlp), header(rlpToData(_rlp))
{
    // string headerInfo = header.getData().asJson() + " vs ";
    // std::cerr << _rlp << std::endl;
    ETH_ERROR_REQUIRE_MESSAGE(header.getData().atKey("hash").asString() == header.hash(),
        "Hash calculated from raw RLP != hash calculated from fields! ");  //(" + headerInfo + ")");

    static const u256 minDiff = u256("0x20000");
    static const u256 maxGasLimit = u256("0x7fffffffffffffff");  // 2**63-1
    ETH_ERROR_REQUIRE_MESSAGE(u256(header.difficulty()) >= minDiff,
        "Block difficulty is too low! ~" + header.difficulty());
    ETH_ERROR_REQUIRE_MESSAGE(
        u256(header.gasLimit()) <= maxGasLimit, "Block gasLimit is too high!");
}

TransactionFromRLP::TransactionFromRLP(dev::RLP const& _rlp)
  : m_validator(_rlp), transaction(rlpToData(_rlp))
{}

BlockHeadFromRLP::validator::validator(dev::RLP const& _rlp)
{
    if (!_rlp.isList())
        throw dev::RLPException("RLP blockHeader is expected to be list");

    for (size_t i = 0; i < 15; i++)
    {
        if (!_rlp[i].isData())
            throw dev::RLPException("Blockheader field is not data!");
    }
}

TransactionFromRLP::validator::validator(dev::RLP const& _rlp)
{
    if (!_rlp.isList())
        throw dev::RLPException("Transaction RLP is expected to be list");

    for (size_t i = 0; i < 9; i++)
    {
        if (!_rlp[i].isData())
            throw dev::RLPException("Transaction rlp field is not data!");
    }
}

scheme_RPCBlock BlockHeadFromRLP::getRPCResponse() const
{
    DataObject response;
    response.replace(header.getData());
    response.renameKey("bloom", "logsBloom");
    response.renameKey("coinbase", "author");
    response["miner"] = response.atKey("author").asString();
    response.renameKey("receiptTrie", "receiptsRoot");
    response.renameKey("transactionsTrie", "transactionsRoot");
    response.renameKey("uncleHash", "sha3Uncles");
    response["size"] = "";
    response["transactions"] = DataObject(DataType::Array);
    response["uncles"] = DataObject(DataType::Array);
    return scheme_RPCBlock(response);
}

DataObject BlockHeadFromRLP::rlpToData(RLP const& _rlp) const
{
    // 0 - parentHash           // 8 - number
    // 1 - uncleHash            // 9 - gasLimit
    // 2 - coinbase             // 10 - gasUsed
    // 3 - stateRoot            // 11 - timestamp
    // 4 - transactionTrie      // 12 - extraData
    // 5 - receiptTrie          // 13 - mixHash
    // 6 - bloom                // 14 - nonce
    // 7 - difficulty
    DataObject bData;
    size_t i = 0;
    bData["parentHash"] = rlpToString(_rlp[i++]);
    bData["uncleHash"] = rlpToString(_rlp[i++]);
    bData["coinbase"] = rlpToString(_rlp[i++]);
    bData["stateRoot"] = rlpToString(_rlp[i++]);
    bData["transactionsTrie"] = rlpToString(_rlp[i++]);
    bData["receiptTrie"] = rlpToString(_rlp[i++]);
    bData["bloom"] = rlpToString(_rlp[i++]);
    bData["difficulty"] = rlpToString(_rlp[i++]);
    bData["number"] = rlpToString(_rlp[i++]);
    bData["gasLimit"] = rlpToString(_rlp[i++]);
    bData["gasUsed"] = rlpToString(_rlp[i++]);
    bData["timestamp"] = rlpToString(_rlp[i++]);

    string const exData = rlpToString(_rlp[i++]);
    bData["extraData"] = (exData == "0x0") ? "" : exData;
    bData["mixHash"] = rlpToString(_rlp[i++]);
    bData["nonce"] = rlpToString(_rlp[i++]);
    bData["hash"] = dev::toHexPrefixed(dev::sha3(_rlp.data()));
    bData.performModifier(mod_valuesToLowerCase);
    // m_currentBlockHeader.header.performModifier(mod_removeLeadingZerosFromHexValuesEVEN);
    // m_currentBlockHeader.header.performVerifier(test::ver_checkHash32Fields);
    return bData;
}

DataObject TransactionFromRLP::rlpToData(RLP const& _rlp)
{
    // 0 - nonce        3 - to      6 - v
    // 1 - gasPrice     4 - value   7 - r
    // 2 - gasLimit     5 - data    8 - s
    DataObject bData;
    size_t i = 0;
    bData["nonce"] = rlpToString(_rlp[i++]);
    bData["gasPrice"] = rlpToString(_rlp[i++]);
    bData["gasLimit"] = rlpToString(_rlp[i++]);
    bData["to"] = rlpToString(_rlp[i++]);
    bData["to"] = bData["to"].asString() == "0x0" ? "" : bData["to"].asString();
    bData["value"] = rlpToString(_rlp[i++]);
    string const data = rlpToString(_rlp[i++]);
    bData["data"] = data == "0x0" ? "" : data;
    bData["v"] = rlpToString(_rlp[i++]);
    bData["r"] = rlpToString(_rlp[i++]);
    bData["s"] = rlpToString(_rlp[i++]);

    bData.performModifier(mod_valuesToLowerCase);
    return bData;
}
}  // namespace toolimpl

using namespace toolimpl;
// Heavy ToolImpl functions

// Verify blockchain logic of a raw block rlp
void ToolImpl::verifyRawBlock(BlockHeadFromRLP const& _sanHeader, dev::RLP const& _blockRLP)
{
    // Take Hash directly from RLP data to be safe
    int rawImportNumber = test::hexOrDecStringToInt(_sanHeader.header.number());
    ETH_ERROR_REQUIRE_MESSAGE(rawImportNumber < 10000, "Sanitize blocknumber convertion error!");

    // Check if we know parent hash and do chain reorg
    ETH_TEST_MESSAGE("Attempt to import number `" + _sanHeader.header.number() +
                     "` on top of chain size `" + toString(getCurrChain().size()));
    ETH_TEST_MESSAGE("ParentHash: " + _sanHeader.header.parentHash());

    // Construct information about current blockheader from ToolImpl prespective
    m_currentBlockHeader.reset();
    m_currentBlockHeader.header["bloom"] = _sanHeader.header.bloom();
    m_currentBlockHeader.header["parentHash"] = _sanHeader.header.parentHash();
    m_currentBlockHeader.header["author"] = _sanHeader.header.coinbase();
    m_currentBlockHeader.header["difficulty"] = _sanHeader.header.difficulty();
    m_currentBlockHeader.header["gasLimit"] = _sanHeader.header.gasLimit();
    m_currentBlockHeader.header["extraData"] = _sanHeader.header.extraData();
    m_currentBlockHeader.header["timestamp"] = _sanHeader.header.timestamp();
    m_currentBlockHeader.header["nonce"] = _sanHeader.header.nonce();
    m_currentBlockHeader.header["mixHash"] = _sanHeader.header.mixHash();
    m_currentBlockHeader.isImportRawBlock = true;

    m_currentBlockHeader.currentBlockNumber = test::hexOrDecStringToInt(_sanHeader.header.number());
    m_currentBlockHeader.timestamp = test::hexOrDecStringToInt(_sanHeader.header.timestamp());

    // block transactions
    m_transactions.clear();
    if (!_blockRLP[1].isList())
        throw dev::RLPException("Transactions RLP is expected to be list!");

    // Caclulate uncle hash
    if (!_blockRLP[2].isList())
        throw dev::RLPException("Uncles expected to be list");

    for (auto const& uncl : _blockRLP[2])
    {
        if (m_currentBlockHeader.uncles.size() == 2)
            throw dev::RLPException("Too many uncles in the block!");

        BlockHeadFromRLP sanUncleHeader(uncl);

        if (test::hexOrDecStringToInt(sanUncleHeader.header.number()) == rawImportNumber)
            throw dev::RLPException("Uncle has the same number as the block being imported!");

        string const& unclePHash = sanUncleHeader.header.parentHash();
        string const& uncleHash = sanUncleHeader.header.hash();
        if (unclePHash == _sanHeader.header.parentHash())
            throw dev::RLPException("Uncle is brother (derived from the same block)");
        bool foundUncestor = false;
        size_t uncestorNumber = 0;
        // search in current chain
        if (getGenesis().getHash() == unclePHash)
        {
            foundUncestor = true;  // check genesis
            uncestorNumber = getGenesis().getNumber();
        }
        else
        {
            for (auto const& bl : getCurrChain())
            {
                if (bl.getHash() == unclePHash)
                {
                    foundUncestor = true;
                    uncestorNumber = bl.getNumber();
                    break;
                }
            }
        }
        if (test::hexOrDecStringToInt(sanUncleHeader.header.number()) > rawImportNumber)
            throw dev::RLPException(
                "Uncle number is in the future: " + sanUncleHeader.header.number());
        if (rawImportNumber - uncestorNumber > 7)
            throw dev::RLPException("Uncle is too old (derived from old block)! HostBlockN: `" +
                                    toString(rawImportNumber) + "` , UncleUncestorN: `" +
                                    toString(uncestorNumber) + "`");
        if (!foundUncestor)
            throw dev::RLPException("Uncle is derived from unknown block!");

        // check dublicated uncles
        for (auto const& bl : getCurrChain())
        {
            if (bl.getHash() == uncleHash)
                throw dev::RLPException("Uncle is one of the blocks imported to chain!");

            for (auto const& un : bl.getRPCResponse().getUncles())
            {
                if (un.asString() == uncleHash)
                    throw dev::RLPException("Dublicate uncles in chain");
            }
        }
        for (auto const& uncle : m_currentBlockHeader.uncles)
        {
            if (uncle.getBlockHash() == uncleHash)
                throw dev::RLPException("Uncle already been attached to this block!");
        }
        m_currentBlockHeader.uncles.push_back(sanUncleHeader.getRPCResponse());
    }

    // if block is valid
    // if block is from fork. make a new chain
    if (test::hexOrDecStringToInt(_sanHeader.header.number()) == 1)
        makeForkForBlockWithPHash(getGenesis().getHash());
    else
        makeForkForBlockWithPHash(_sanHeader.header.parentHash());

    // Chek header values sanity
    const scheme_blockHeader lastBlock = getLastBlock().getRPCResponse().getBlockHeader2();
    u256 timeDiff = u256(_sanHeader.header.timestamp()) - u256(lastBlock.timestamp());
    ETH_ERROR_REQUIRE_MESSAGE(timeDiff > 0,
        "Rawblock timestamp is too fast compared to its parent! ~" + toString(timeDiff));
    // u256 gasLimitDiff = u256(_sanHeader.header.gasLimit()) - u256(lastBlock.gasLimit());
    // ETH_ERROR_REQUIRE_MESSAGE(gasLimitDiff <= 1000, "Rawblock gasLimit change wrong compared to
    // its parent! ~" + toString(gasLimitDiff));


    ETH_ERROR_REQUIRE_MESSAGE((int)getCurrChain().size() + 1 == rawImportNumber,
        string("If we import Rawblock with number >= 1, blockchain must have `number` blocks! /n") +
            "Blockchain size: " + to_string(getCurrChain().size()) +
            " BlockImportN: " + _sanHeader.header.number());

    // Check that parent hash of rawImportedBlock is equal to previous block known
    if (rawImportNumber == 1)
    {
        ETH_ERROR_REQUIRE_MESSAGE(getGenesis().getHash() == _sanHeader.header.parentHash(),
            "Unknown genesis: " + _sanHeader.header.parentHash());
    }
    else
    {
        ETH_ERROR_REQUIRE_MESSAGE(rawImportNumber > 1, "ImportRaw block require number >= 1");
        ToolBlock const& pbl = getLastBlock();  // previous block
        ETH_ERROR_REQUIRE_MESSAGE(pbl.getHash() == _sanHeader.header.parentHash(),
            "ImportRaw block require previous block hash == block.parentHash");
    }

    //for (auto const& tr : _blockRLP[1])
    //    eth_sendRawTransaction(TransactionFromRLP(tr).transaction);
}


// Make an RPC like response using data that we know from tool
scheme_RPCBlock ToolImpl::internalConstructResponseGetBlockByHashOrNumber(
    DataObject const& _toolResponse)
{
    DataObject constructResponse(DataType::Null);
    scheme_toolResponse toolResponse(_toolResponse);
    DataObject const& rdata = toolResponse.getData();
    DataObject const& currentHeader = m_currentBlockHeader.header;
    size_t receiptSize = rdata.atKey("receipts").getSubObjects().size();

    DataObject lastReceipt;
    if (receiptSize > 0)
    {
        DataObject const& _lastReceipt =
            rdata.atKey("receipts")
                .getSubObjects()
                .at(rdata.atKey("receipts").getSubObjects().size() - 1);
        lastReceipt["logsBloom"] = _lastReceipt.atKey("logsBloom");

        // Construct fake blockheader from all the info we know
        // Env of block execution from test
        // TooResponse for state hashes and transactions

        // Get all gas usage provided by tool
        dev::u256 maxCommGasUsed = 0;
        for (auto const& receipt : rdata.atKey("receipts").getSubObjects())
        {
            // Find transaction that was sent
            scheme_transaction const* trScheme = 0;
            for (auto const& tr : m_transactions)
            {
                if (tr.getHash() == receipt.atKey("transactionHash").asString())
                    trScheme = &tr;
            }
            ETH_ERROR_REQUIRE_MESSAGE(
                trScheme != 0, "Tool return transaction hash that is missing in m_transactions");

            // Fill full transaction object from all the info we know (rpc style)
            // m_transactions is raw info about transactions that was requested to be setn
            // receipts are the valid transaction that were executed by tool
            DataObject fullTransaction;
            fullTransaction["blockHash"] =
                dev::toHexPrefixed(h256(0));  // update when we know the hash
            fullTransaction["blockNumber"] =
                dev::toCompactHexPrefixed(m_currentBlockHeader.currentBlockNumber, 1);
            fullTransaction["from"] = "0xa94f5374fce5edbc8e2a8697c15331677e6ebf0b";  // recover vrs
            fullTransaction["gas"] = trScheme->getData().atKey("gasLimit").asString();
            fullTransaction["gasPrice"] = trScheme->getData().atKey("gasPrice").asString();
            fullTransaction["hash"] = receipt.atKey("transactionHash").asString();
            fullTransaction["input"] = trScheme->getData().atKey("data").asString();
            fullTransaction["nonce"] = trScheme->getData().atKey("nonce").asString();
            fullTransaction["to"] = trScheme->getData().atKey("to").asString();
            fullTransaction["transactionIndex"] = receipt.atKey("transactionIndex").asString();
            fullTransaction["value"] = trScheme->getData().atKey("value").asString();
            if (trScheme->getData().count("v"))
            {
                fullTransaction["v"] = trScheme->getData().atKey("v").asString();
                fullTransaction["r"] = trScheme->getData().atKey("r").asString();
                fullTransaction["s"] = trScheme->getData().atKey("s").asString();
            }
            else
            {
                fullTransaction["v"] = "0x00";
                fullTransaction["r"] = "0x00";
                fullTransaction["s"] = "0x00";
            }
            constructResponse["transactions"].addArrayObject(fullTransaction);

            u256 commGasUsed = u256(receipt.atKey("cumulativeGasUsed").asString());
            maxCommGasUsed = (commGasUsed > maxCommGasUsed) ? commGasUsed : maxCommGasUsed;
        }
        lastReceipt["maxCommGasUsed"] = dev::toCompactHexPrefixed(maxCommGasUsed, 1);
    }
    else
    {
        // Default values. (the genesis block contain no receipts) so use defaults
        lastReceipt["logsBloom"] =
            "0x000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
            "00000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
            "00000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
            "00000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
            "00000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
            "000000000000000000000000000000000000000000000000000000000000000000000000000000000000";
        lastReceipt["maxCommGasUsed"] = "0x00";
        constructResponse["transactions"] = DataObject(DataType::Array);
    }

    string const exData = currentHeader.atKey("extraData").asString();
    constructResponse["extraData"] = exData.empty() ? "0x" : exData;
    constructResponse["gasLimit"] = currentHeader.atKey("gasLimit");
    constructResponse["gasUsed"] = lastReceipt.atKey("maxCommGasUsed").asString();
    constructResponse["logsBloom"] = lastReceipt.atKey("logsBloom");
    constructResponse["miner"] = currentHeader.atKey("author");
    constructResponse["number"] =
        dev::toCompactHexPrefixed(m_currentBlockHeader.currentBlockNumber, 1);
    constructResponse["receiptsRoot"] = rdata.atKey("receiptRoot");
    constructResponse["stateRoot"] = rdata.atKey("stateRoot");
    constructResponse["timestamp"] = dev::toCompactHexPrefixed(m_currentBlockHeader.timestamp, 1);
    constructResponse["uncles"] = DataObject(DataType::Array);
    for (auto const& un : m_currentBlockHeader.uncles)
        constructResponse["uncles"].addArrayObject(un.getBlockHash());
    constructResponse["transactionsRoot"] = rdata.atKey("txRoot");
    constructResponse["difficulty"] = currentHeader.atKey("difficulty");
    constructResponse["nonce"] = currentHeader.atKey("nonce");
    constructResponse["mixHash"] = currentHeader.atKey("mixHash");

    constructResponse["hash"] =
        dev::toHexPrefixed(h256(0));  // Recalculate after with overwriteBlockHeader
    constructResponse["sha3Uncles"] =
        dev::toHexPrefixed(h256(0));  // Recalculate after with overwriteBlockHeader
    constructResponse["parentHash"] = m_currentBlockHeader.header.atKey("parentHash");
    constructResponse["size"] = "0x00";  // Not Needed";
    constructResponse.performModifier(mod_valuesToLowerCase);

    scheme_RPCBlock rpcBlock(constructResponse);
    for (auto const& un : m_currentBlockHeader.uncles)
        rpcBlock.addUncle(un);
    rpcBlock.recalculateUncleHash();
    rpcBlock.overwriteBlockHeader(rpcBlock.getBlockHeader());
    rpcBlock.setLogsHash(rdata.atKey("logsHash").asString());

    // Update transaction info in block rpc response
    rpcBlock.tool_updateTransactionInfo();

    return rpcBlock;
}

void ToolImpl::doChainReorg()
{
    // Select highest total difficulty chain
    dev::u256 maxTotalDiff = 0;
    for (auto const& chain : m_blockchainMap)
    {
        dev::u256 tch_d = 0;
        ToolChain const& tch = chain.second;
        for (auto const& bl : tch)
            tch_d += u256(bl.getRPCResponse().getBlockHeader2().difficulty());

        if (tch_d > maxTotalDiff)
        {
            m_current_chain_ind = chain.first;
            maxTotalDiff = tch_d;
        }
    }
}

void ToolImpl::makeForkForBlockWithPHash(string const& _parentHash)
{
    // Look for _parentHash in chain database
    // if hash is found make that chain current
    // if hash is below top head of found chain, make a new chain and make that chain current
    bool found = false;
    if (getGenesis().getHash() == _parentHash)
    {
        // Parent is genesis, make new fork starting from genesis
        m_current_chain_ind = ++m_maxChainID;
        m_blockchainMap[m_current_chain_ind].size();
        return;
    }

    for (auto const& chain : m_blockchainMap)
    {
        ToolChain const& tch = chain.second;
        for (auto const& bl : tch)
        {
            if (bl.getHash() == _parentHash)
            {
                found = true;

                // if it is last block in chain
                if (tch.size() == (size_t)bl.getNumber())
                    m_current_chain_ind = chain.first;
                else
                {
                    // make a new fork
                    m_current_chain_ind = ++m_maxChainID;
                    ETH_ERROR_REQUIRE_MESSAGE((size_t)bl.getNumber() <= tch.size(),
                        "bl in chain number must be <= chain size");
                    // Copy up to fork block
                    for (size_t i = 0; i < (size_t)bl.getNumber(); i++)
                        m_blockchainMap[m_current_chain_ind].push_back(tch.at(i));
                }
                break;
            }
        }
        if (found)
            break;
    }

    if (!found)
        ETH_ERROR_MESSAGE("ParentHash not found: " + _parentHash);
}

ToolImpl::ToolBlock const& ToolImpl::getBlockByHashOrNumber(string const& _hashOrNumber) const
{
    if (_hashOrNumber.size() == 66)
    {
        // need to look up all chains !!!
        for (size_t i = 0; i < getCurrChain().size(); i++)
            if (getCurrChain().at(i).getHash() == _hashOrNumber)
                return getCurrChain().at(i);
    }
    else
    {
        int bNumber = hexOrDecStringToInt(_hashOrNumber);
        ETH_ERROR_REQUIRE_MESSAGE(
            bNumber >= 0 && bNumber < 10000, "getBlockByHashOrNumber sanitize block number!");
        if (bNumber == 0)
            return getGenesis();
        bNumber--;  // chain vector starts with 0
        if (bNumber >= 0 && (size_t)bNumber < getCurrChain().size())
            return getCurrChain().at(bNumber);
    }
    ETH_ERROR_MESSAGE("Wrong block hash or number! " + _hashOrNumber);
    return getCurrChain().at(0);
}

void ToolImpl::onError(string const& _what, string const& _message)
{
    // exception happend when processing block
    m_transactions.clear();
    m_currentBlockHeader.reset();
    doChainReorg();  // to stay safe
    m_lastInterfaceError["message"] = _message;
    m_lastInterfaceError["error"] = string("Error parsing block: ") + _what;
    ETH_TEST_MESSAGE(m_lastInterfaceError.asJson());
}

*/
