#pragma once
#include "../object.h"
#include <libdevcore/Address.h>
#include <libdevcore/RLP.h>
using namespace dev;

namespace test {
	class scheme_block: public object
	{
		public:
		scheme_block(DataObject const& _block):
			object(_block)
		{
            requireJsonFields(_block, "blockRPC",
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
                    {"difficulty", {{DataType::String}, jsonField::Optional}},
                    {"seedHash", {{DataType::String}, jsonField::Optional}},
                    {"nonce", {{DataType::String}, jsonField::Optional}},
                    {"mixHash", {{DataType::String}, jsonField::Optional}}});

            for (auto const& trObj : m_data.at("transactions").getSubObjects())
            {
                requireJsonFields(trObj, "block rpc",
                    {{"blockHash", {DataType::String}}, {"blockNumber", {DataType::String}},
                        {"from", {DataType::String}}, {"gas", {DataType::String}},
                        {"gasPrice", {DataType::String}}, {"hash", {DataType::String}},
                        {"input", {DataType::String}}, {"nonce", {DataType::String}},
                        {"to", {DataType::String, DataType::Null}}, {"v", {DataType::String}},
                        {"r", {DataType::String}}, {"s", {DataType::String}},
                        {"transactionIndex", {DataType::String}}, {"value", {DataType::String}}});
            }
        }

		int getTransactionCount() const { return m_data.at("transactions").getSubObjects().size(); }
        std::string getBlockHash() const { return m_data.at("hash").asString(); }

        // Get Block RLP for state tests
        std::string getBlockRLP() const
        {
            // RLP of a block
            // rlpHead .. blockinfo transactions uncles
            RLPStream stream(3);
            RLPStream header;
            header.appendList(15);
            header << h256(m_data.at("parentHash").asString());
            header << h256(m_data.at("sha3Uncles").asString());
            header << dev::Address(m_data.at("author").asString());
            header << h256(m_data.at("stateRoot").asString());
            header << h256(m_data.at("transactionsRoot").asString());
            header << h256(m_data.at("receiptsRoot").asString());
            header << h2048(m_data.at("logsBloom").asString());
            header << u256(m_data.at("totalDifficulty").asString());
            header << u256(m_data.at("number").asString());
            header << u256(m_data.at("gasLimit").asString());
            header << u256(m_data.at("gasUsed").asString());
            header << u256(m_data.at("timestamp").asString());
            header << dev::fromHex(m_data.at("extraData").asString());
            if (m_data.count("mixHash"))
            {
                header << h256(m_data.at("mixHash").asString());
                header << u256(m_data.at("nonce").asString());
            }
            else
            {
                header << h256(0);
                header << u256(0);
            }
            stream.appendRaw(header.out());

            if (m_data.at("transactions").getSubObjects().size())
            {
                RLPStream transactionList(1);
                RLPStream transactionRLP(9);
                DataObject transaction = m_data.at("transactions").getSubObjects().at(0);
                transactionRLP << u256(transaction.at("nonce").asString());
                transactionRLP << u256(transaction.at("gasPrice").asString());
                transactionRLP << u256(transaction.at("gas").asString());
                if (transaction.at("to").type() == DataType::Null ||
                    transaction.at("to").asString().empty())
                    transactionRLP << "";
                else
                    transactionRLP << Address(transaction.at("to").asString());
                transactionRLP << u256(transaction.at("value").asString());
                transactionRLP << fromHex(transaction.at("input").asString());

                byte v = 27 + (int)u256(transaction.at("v").asString().c_str());
                transactionRLP << v;
                transactionRLP << u256(transaction.at("r").asString());
                transactionRLP << u256(transaction.at("s").asString());
                transactionList.appendRaw(transactionRLP.out());
                stream.appendRaw(transactionList.out());
            }
            else
                stream.appendRaw(RLPStream(0).out());  // empty transaction list
            stream.appendRaw(RLPStream(0).out());  // empty uncle list

            return dev::toHexPrefixed(stream.out());
        }
    };
}

