#pragma once
#include "../object.h"
#include <libdevcore/Address.h>
#include <libdevcore/RLP.h>
#include <libdevcore/SHA3.h>

using namespace dev;

namespace test {
// TODO (unify the blocknumber conversion througout the codebase and remove this class !!! )
// Represent int value taken from either size_t or string
// And perform additive operations on it with another (int or string representing integers)
struct BlockNumber
{
    BlockNumber(string const& _number) : m_blockNumber(_number) {}
    BlockNumber(size_t _number) : m_blockNumber("") { m_blockNumber = toString(_number); }
    int getBlockNumberAsInt() const { return hexOrDecStringToInt(m_blockNumber); }
    string getBlockNumberAsString() const { return m_blockNumber; }
    void applyShift(int _shift)
    {
        m_blockNumber = toCompactHexPrefixed(getBlockNumberAsInt() + _shift, 1);
    }
    void applyShift(string const& _shift)
    {
        int shift = hexOrDecStringToInt(_shift);
        m_blockNumber = toCompactHexPrefixed(getBlockNumberAsInt() + shift, 1);
    }

private:
    string m_blockNumber;
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
            resetHeader(mappedHeader);
        }

        void overwriteBlockHeader(DataObject const& _header) { resetHeader(_header); }

        void replaceUncleHash(string const& _string)
        {
            m_data.atKeyUnsafe("uncleHash") = _string;
            object::makeKeyHex(m_data.atKeyUnsafe("uncleHash"));
        }

        // Change coinbase in blockheader
        void randomizeCoinbase()
        {
            // m_data.atKeyUnsafe("coinbase") = toString(Address(h160::random()));
            // if we randomize coinbase retesteth will be unable to calculate stateRoot
            // state root is applied manually in test as expect section
            m_data.atKeyUnsafe("coinbase") =
                toString(Address("0xb94f5374fce5ed0000000097c15331677e6ebf0b"));
        }

        //
        void removeNonceAndMixhash()
        {
            if (m_data.count("nonce"))
                m_data.removeKey("nonce");
            if (m_data.count("mixHash"))
                m_data.removeKey("mixHash");
        }

    private:
        void resetHeader(DataObject const& _header);

        // Convert between rpc response field names and standard fields
        DataObject mapBlockHeader() const;
    };

    scheme_block(std::string const& _RLP);
    scheme_block(DataObject const& _block);

    void addUncle(scheme_block const& _block) { m_uncles.push_back(_block); }

    void recalculateUncleHash();

    void setValid(bool _isValid) { m_isValid = _isValid; }

    bool isValid() const { return m_isValid; }

    string const& getStateHash() const { return m_data.atKey("stateRoot").asString(); }

    string const& getNumber() const { return m_data.atKey("number").asString(); }

    size_t getTransactionCount() const
    {
        return m_data.atKey("transactions").getSubObjects().size();
    }

    std::vector<DataObject> const& getTransactions() const
    {
        return m_data.atKey("transactions").getSubObjects();
    }

    std::vector<DataObject> const& getUncles() const
    {
        return m_data.atKey("uncles").getSubObjects();
    }

    std::string const& getBlockHash() const { return m_data.atKey("hash").asString(); }

    DataObject const& getBlockHeader() const { return m_blockHeader.getData(); }
    void removeNonceAndMixhash() { m_blockHeader.removeNonceAndMixhash(); }

    void overwriteBlockHeader(DataObject const& _header)
    {
        m_blockHeader.overwriteBlockHeader(_header);
    }

    // Get Block RLP for state tests
    std::string getBlockRLP() const;

    // Change coinbase in blockheader
    void randomizeCoinbase();

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

    static RLPStream streamBlockHeader(DataObject const& _headerData);

private:
    RLPStream streamUncles() const;

    bool m_isFullTransactions = false;
    bool m_isValid = true;
    validator m_validator;
    scheme_block_header m_blockHeader;
    std::vector<scheme_block> m_uncles;
    std::string m_rlpOverride;
};
}

