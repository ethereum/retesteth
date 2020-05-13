#include <retesteth/session/ToolImpl.h>
#include <retesteth/session/ToolImplHelper.h>

namespace toolimpl
{
// DataObject modifiers
void mod_valuesToLowerCase(DataObject& _obj)
{
    if (_obj.type() == DataType::String)
    {
        string value = _obj.asString();
        std::transform(value.begin(), value.end(), value.begin(),
            [](unsigned char c) { return std::tolower(c); });
        _obj = value;
    }
}

// Remove leading zeros from hex values leaving 0x0004 - > 0x04
void mod_removeLeadingZerosFromHexValuesEVEN(DataObject& _obj)
{
    mod_removeLeadingZerosFromHexValues(_obj);
    static std::vector<std::string> const c_hashes{std::string{"to"}, std::string{"data"}};
    if (_obj.type() == DataType::String && !inArray(c_hashes, _obj.getKey()))
    {
        object::DigitsType t = object::stringIntegerType(_obj.asString());
        if (t == object::DigitsType::UnEvenHexPrefixed)
            _obj.setString("0x0" + _obj.asString().substr(2));
    }
}

string rlpToString(dev::RLP const& _rlp, bool _corretHexOdd)
{
    std::ostringstream stream;
    stream << _rlp.toBytes();
    if (stream.str() == "0x")
        return (_corretHexOdd) ? "0x00" : "0x0";
    return stream.str();
}

// Block Header Sanitizer when parsing RLP
BlockHeadFromRLP::BlockHeadFromRLP(dev::RLP const& _rlp)
  : m_validator(_rlp), header(rlpToData(_rlp))
{}
/*
if (!object::validateHash(parentHash, 32))
    throw dev::RLPException("parentHash is not hash32");

if (bloom.size() != 256*2+2)
    throw dev::RLPException("Bloom is wrong size! (" + toString(bloom.size()) + ")");

if (extraData.size() > 66)  // 0x + 32 bytes
    throw dev::RLPException("extraData is too long");
*/
//}

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

DataObject BlockHeadFromRLP::rlpToData(RLP const& _rlp)
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
    bData["extraData"] = rlpToString(_rlp[i++]);
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
    bData["value"] = rlpToString(_rlp[i++]);
    std::ostringstream stream;
    stream << _rlp[5];
    i++;
    bData["data"] = stream.str() == "0x0" ? "" : rlpToString(_rlp[5], true);
    bData["v"] = rlpToString(_rlp[i++]);
    bData["r"] = rlpToString(_rlp[i++]);
    bData["s"] = rlpToString(_rlp[i++]);
    bData.performModifier(mod_valuesToLowerCase);
    return bData;
}

}  // namespace toolimpl

using namespace toolimpl;
// Heavy ToolImpl functions
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
            ETH_ERROR_REQUIRE_MESSAGE(trScheme != 0, "Not found transaction in m_transactions");

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

    constructResponse["extraData"] = currentHeader.atKey("extraData");
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
            tch_d += u256(bl.getRPCResponse().getBlockHeader().atKey("difficulty").asString());

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
