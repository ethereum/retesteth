#pragma once
#include "../object.h"
#include <libdevcore/Address.h>
#include <libdevcore/RLP.h>
using namespace dev;

namespace test {
struct BlockNumber
{
    BlockNumber(string const& _number) : m_blockNumber(_number) {}
    int getBlockNumberAsInt() const { return hexStringToInt(m_blockNumber); }
    string getBlockNumberAsString() const { return m_blockNumber; }
    void applyShift(int _shift)
    {
        m_blockNumber = toCompactHexPrefixed(getBlockNumberAsInt() + _shift);
    }
    void applyShift(string const& _shift)
    {
        m_blockNumber = toCompactHexPrefixed(getBlockNumberAsInt() + hexStringToInt(_shift));
    }

private:
    string m_blockNumber;
    int hexStringToInt(string const& _str) const { return (int)u256(fromHex(_str)); }
};

class scheme_block : public object
{
public:
    class scheme_block_header : public object
    {
    public:
        // assume that _header was already verified by scheme_block
        scheme_block_header(DataObject const& _header) : object(_header)
        {
            // translate rpc response of block header into standard block header
            DataObject mappedHeader = mapBlockHeader();
            requireJsonFields(mappedHeader, "scheme_block_header",
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
            m_data = mappedHeader;
        }

    private:
        DataObject mapBlockHeader() const
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
                header["mixHash"] =
                    "0x0000000000000000000000000000000000000000000000000000000000000000";
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
    };

    scheme_block(DataObject const& _block)
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
                requireJsonFields(trObj, "block rpc transaction element",
                    {{"blockHash", {DataType::String}}, {"blockNumber", {DataType::String}},
                        {"from", {DataType::String}}, {"gas", {DataType::String}},
                        {"gasPrice", {DataType::String}}, {"hash", {DataType::String}},
                        {"input", {DataType::String}}, {"nonce", {DataType::String}},
                        {"to", {DataType::String, DataType::Null}}, {"v", {DataType::String}},
                        {"r", {DataType::String}}, {"s", {DataType::String}},
                        {"transactionIndex", {DataType::String}}, {"value", {DataType::String}}});
        else
            for (auto const& trObj : m_data.atKey("transactions").getSubObjects())
                ETH_ERROR_REQUIRE_MESSAGE(trObj.type() == DataType::String,
                    "block rpc transaction element is expected to be hash string!");
    }

    void setValid(bool _isValid) { m_isValid = _isValid; }

    bool isValid() const { return m_isValid; }

    string const& getStateHash() const { return m_data.atKey("stateRoot").asString(); }

    string const& getNumber() const { return m_data.atKey("number").asString(); }

    size_t getTransactionCount() const
    {
        return m_data.atKey("transactions").getSubObjects().size();
    }
    std::string const& getBlockHash() const { return m_data.atKey("hash").asString(); }

    DataObject getBlockHeader() const { return m_blockHeader.getData(); }

    void overwriteBlockHeader(DataObject const& _header)
    {
        m_data["logsBloom"] = _header.atKey("bloom").asString();
        m_data["author"] = _header.atKey("coinbase").asString();
        m_data["difficulty"] = _header.atKey("difficulty");
        m_data["extraData"] = _header.atKey("extraData");
        m_data["gasLimit"] = _header.atKey("gasLimit");
        m_data["gasUsed"] = _header.atKey("gasUsed");
        m_data["hash"] = _header.atKey("hash");
        m_data["mixHash"] = _header.atKey("mixHash");
        m_data["nonce"] = _header.atKey("nonce");

        m_data["number"] = _header.atKey("number");
        m_data["parentHash"] = _header.atKey("parentHash");
        m_data["receiptsRoot"] = _header.atKey("receiptTrie").asString();
        m_data["stateRoot"] = _header.atKey("stateRoot");
        m_data["timestamp"] = _header.atKey("timestamp");
        m_data["transactionsRoot"] = _header.atKey("transactionsTrie").asString();
        m_data["sha3Uncles"] = _header.atKey("uncleHash").asString();
        // makeAllFieldsHex(m_data);
    }

    // Get Block RLP for state tests
    std::string getBlockRLP() const
    {
        ETH_ERROR_REQUIRE_MESSAGE(m_isFullTransactions,
            "Attempt to get blockRLP of a block received without full transactions!");
        // RLP of a block
        // rlpHead .. blockinfo transactions uncles
        RLPStream stream(3);
        RLPStream header;
        header.appendList(15);
        header << h256(m_data.atKey("parentHash").asString());
        header << h256(m_data.atKey("sha3Uncles").asString());
        header << dev::Address(m_data.atKey("author").asString());
        header << h256(m_data.atKey("stateRoot").asString());
        header << h256(m_data.atKey("transactionsRoot").asString());
        header << h256(m_data.atKey("receiptsRoot").asString());
        header << h2048(m_data.atKey("logsBloom").asString());
        header << u256(m_data.atKey("difficulty").asString());
        header << u256(m_data.atKey("number").asString());
        header << u256(m_data.atKey("gasLimit").asString());
        header << u256(m_data.atKey("gasUsed").asString());
        header << u256(m_data.atKey("timestamp").asString());
        header << dev::fromHex(m_data.atKey("extraData").asString());
        if (m_data.count("mixHash"))
        {
            header << h256(m_data.atKey("mixHash").asString());
            header << h64(m_data.atKey("nonce").asString());
        }
        else
        {
            header << h256(0);
            header << h64(0);
        }
        stream.appendRaw(header.out());

        size_t trCount = m_data.atKey("transactions").getSubObjects().size();
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
        stream.appendRaw(transactionList.out());

        stream.appendRaw(RLPStream(0).out());  // empty uncle list
        /*
            RLPStream uncleStream;
            uncleStream.appendList(m_uncles.size());
            for (unsigned i = 0; i < m_uncles.size(); ++i)
            {
                RLPStream uncleRlp;
                m_uncles[i].blockHeader().streamRLP(uncleRlp);
                uncleStream.appendRaw(uncleRlp.out());
            }
        */

        return dev::toHexPrefixed(stream.out());
    }

private:
    struct validator
    {
        validator(DataObject const& _data)
        {
            // validate rpc response on eth_getBlock()
            requireJsonFields(_data, "blockRPC",
                {{"author", {{DataType::String}, jsonField::Required}},
                    {"extraData", {{DataType::String}, jsonField::Required}},
                    {"gasLimit", {{DataType::String}, jsonField::Required}},
                    {"gasUsed", {{DataType::String}, jsonField::Required}},
                    {"hash", {{DataType::String}, jsonField::Required}},
                    {"logsBloom", {{DataType::String}, jsonField::Required}},
                    {"miner", {{DataType::String}, jsonField::Required}},
                    {"number", {{DataType::String}, jsonField::Required}},
                    {"parentHash", {{DataType::String}, jsonField::Required}},
                    {"receiptsRoot", {{DataType::String}, jsonField::Required}},
                    {"sha3Uncles", {{DataType::String}, jsonField::Required}},
                    {"size", {{DataType::String}, jsonField::Required}},
                    {"stateRoot", {{DataType::String}, jsonField::Required}},
                    {"timestamp", {{DataType::String}, jsonField::Required}},
                    {"totalDifficulty", {{DataType::String}, jsonField::Required}},
                    {"transactions", {{DataType::Array}, jsonField::Required}},
                    {"transactionsRoot", {{DataType::String}, jsonField::Required}},
                    {"uncles", {{DataType::Array}, jsonField::Required}},
                    {"boundary", {{DataType::String}, jsonField::Optional}},
                    {"difficulty", {{DataType::String}, jsonField::Required}},
                    {"seedHash", {{DataType::String}, jsonField::Optional}},
                    {"nonce", {{DataType::String}, jsonField::Optional}},
                    {"mixHash", {{DataType::String}, jsonField::Optional}}});
        }
    };

private:
    bool m_isFullTransactions = false;
    bool m_isValid = true;
    validator m_validator;
    scheme_block_header m_blockHeader;
};
}

