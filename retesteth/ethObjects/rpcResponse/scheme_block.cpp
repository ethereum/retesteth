#include "scheme_block.h"

namespace test
{
scheme_block::scheme_block(DataObject const& _block)
  : object(_block), m_validator(_block), m_blockHeader(_block)
{
    if (m_data.atKey("transactions").getSubObjects().size())
        m_isFullTransactions =
            m_data.atKey("transactions").getSubObjects().at(0).count("blockHash");
    else
    {
        m_isFullTransactions = true;
        return;
    }

    m_isValid = true;
    if (m_isFullTransactions)
        for (auto const& trObj : m_data.atKey("transactions").getSubObjects())
        {
            requireJsonFields(trObj, "block rpc transaction element",
                {{"blockHash", {DataType::String}}, {"blockNumber", {DataType::String}},
                    {"from", {DataType::String}}, {"gas", {DataType::String}},
                    {"gasPrice", {DataType::String}}, {"hash", {DataType::String}},
                    {"input", {DataType::String}}, {"nonce", {DataType::String}},
                    {"to", {DataType::String, DataType::Null}}, {"v", {DataType::String}},
                    {"r", {DataType::String}}, {"s", {DataType::String}},
                    {"transactionIndex", {DataType::String}}, {"value", {DataType::String}}});
        }
    else
        for (auto const& trObj : m_data.atKey("transactions").getSubObjects())
            ETH_ERROR_REQUIRE_MESSAGE(trObj.type() == DataType::String,
                "block rpc transaction element is expected to be hash string!");
}

// Get Block RLP for state tests
std::string scheme_block::getBlockRLP() const
{
    ETH_ERROR_REQUIRE_MESSAGE(m_isFullTransactions,
        "Attempt to get blockRLP of a block received without full transactions!");
    // RLP of a block
    // rlpHead .. blockinfo transactions uncles
    RLPStream stream(3);
    DataObject const& headerData = m_blockHeader.getData();
    stream.appendRaw(streamBlockHeader(headerData).out());

    size_t trCount = m_data.atKey("transactions").getSubObjects().size();
    // std::cerr << m_data.atKey("transactions").asJson() << std::endl;
    RLPStream transactionList(trCount);
    for (size_t i = 0; i < trCount; i++)
    {
        RLPStream transactionRLP(9);
        DataObject transaction = m_data.atKey("transactions").getSubObjects().at(i);
        transactionRLP << u256(transaction.atKey("nonce").asString());
        transactionRLP << u256(transaction.atKey("gasPrice").asString());
        transactionRLP << u256(transaction.atKey("gas").asString());
        if (transaction.atKey("to").type() == DataType::Null ||
            transaction.atKey("to").asString().empty())
            transactionRLP << "";
        else
            transactionRLP << Address(transaction.atKey("to").asString());
        transactionRLP << u256(transaction.atKey("value").asString());
        transactionRLP << fromHex(transaction.atKey("input").asString());

        byte v = (int)u256(transaction.atKey("v").asString().c_str());
        if (v <= 1)
        {
            v += 27;  // To deal with Aleth's logic to subtract 27 from V when it is 27 or 28
        }
        transactionRLP << v;
        transactionRLP << u256(transaction.atKey("r").asString());
        transactionRLP << u256(transaction.atKey("s").asString());
        transactionList.appendRaw(transactionRLP.out());
    }
    stream.appendRaw(transactionList.out());  // transaction list

    stream.appendRaw(streamUncles().out());  // uncle list

    return dev::toHexPrefixed(stream.out());
}

RLPStream scheme_block::streamUncles() const
{
    RLPStream uncleStream;
    uncleStream.appendList(m_uncles.size());
    for (auto const& bl : m_uncles)
        uncleStream.appendRaw(streamBlockHeader(bl.getBlockHeader()).out());
    return uncleStream;
}

RLPStream scheme_block::streamBlockHeader(DataObject const& _headerData)
{
    RLPStream header;
    header.appendList(15);

    // Map Block Header
    header << h256(_headerData.atKey("parentHash").asString());
    header << h256(_headerData.atKey("uncleHash").asString());
    header << dev::Address(_headerData.atKey("coinbase").asString());
    header << h256(_headerData.atKey("stateRoot").asString());
    header << h256(_headerData.atKey("transactionsTrie").asString());
    header << h256(_headerData.atKey("receiptTrie").asString());
    header << h2048(_headerData.atKey("bloom").asString());
    header << u256(_headerData.atKey("difficulty").asString());
    header << u256(_headerData.atKey("number").asString());
    header << u256(_headerData.atKey("gasLimit").asString());
    header << u256(_headerData.atKey("gasUsed").asString());
    header << u256(_headerData.atKey("timestamp").asString());
    header << dev::fromHex(_headerData.atKey("extraData").asString());
    if (_headerData.count("mixHash"))
    {
        header << h256(_headerData.atKey("mixHash").asString());
        header << h64(_headerData.atKey("nonce").asString());
    }
    else
    {
        header << h256(0);
        header << h64(0);
    }
    return header;
}

void scheme_block::recalculateUncleHash()
{
    m_blockHeader.replaceUncleHash(toString(dev::sha3(streamUncles().out())));
}

void scheme_block::randomizeCoinbase()
{
    m_blockHeader.randomizeCoinbase();
}

// Subclass of blockheader
void scheme_block::scheme_block_header::resetHeader(DataObject const& _header)
{
    requireJsonFields(_header, "scheme_block_header",
        {{"coinbase", {{DataType::String}, jsonField::Required}},
            {"extraData", {{DataType::String}, jsonField::Required}},
            {"gasLimit", {{DataType::String}, jsonField::Required}},
            {"gasUsed", {{DataType::String}, jsonField::Required}},
            {"hash", {{DataType::String}, jsonField::Required}},
            {"bloom", {{DataType::String}, jsonField::Required}},
            {"number", {{DataType::String}, jsonField::Required}},
            {"parentHash", {{DataType::String}, jsonField::Required}},
            {"receiptTrie", {{DataType::String}, jsonField::Required}},
            {"uncleHash", {{DataType::String}, jsonField::Required}},
            {"stateRoot", {{DataType::String}, jsonField::Required}},
            {"timestamp", {{DataType::String}, jsonField::Required}},
            {"transactionsTrie", {{DataType::String}, jsonField::Required}},
            {"difficulty", {{DataType::String}, jsonField::Required}},
            {"nonce", {{DataType::String}, jsonField::Optional}},
            {"mixHash", {{DataType::String}, jsonField::Optional}}});
    m_data.clear();
    m_data = _header;
    makeAllFieldsHex(m_data);
    m_data["hash"] = "0x" + toString(dev::sha3(scheme_block::streamBlockHeader(m_data).out()));
}

DataObject scheme_block::scheme_block_header::mapBlockHeader() const
{
    // Map Block Header
    DataObject header;
    header["bloom"] = m_data.atKey("logsBloom");
    header["coinbase"] = m_data.atKey("author");
    header["difficulty"] = m_data.atKey("difficulty");
    header["extraData"] = m_data.atKey("extraData");
    header["gasLimit"] = m_data.atKey("gasLimit");
    header["gasUsed"] = m_data.atKey("gasUsed");
    header["hash"] = m_data.atKey("hash");
    if (m_data.count("mixHash"))
    {
        header["mixHash"] = m_data.atKey("mixHash");
        header["nonce"] = m_data.atKey("nonce");
    }
    else
    {
        header["mixHash"] = "0x0000000000000000000000000000000000000000000000000000000000000000";
        header["nonce"] = "0x0000000000000000";
    }
    header["number"] = m_data.atKey("number");
    header["parentHash"] = m_data.atKey("parentHash");
    header["receiptTrie"] = m_data.atKey("receiptsRoot");
    header["stateRoot"] = m_data.atKey("stateRoot");
    header["timestamp"] = m_data.atKey("timestamp");
    header["transactionsTrie"] = m_data.atKey("transactionsRoot");
    header["uncleHash"] = m_data.atKey("sha3Uncles");
    return header;
}

}  // namespace test
