#pragma once
#include "../object.h"
#include <libdevcore/RLP.h>
using namespace dev;

namespace test {
	class scheme_block: public object
	{
		public:
		scheme_block(DataObject const& _block):
			object(_block)
		{
			requireJsonFields(_block, "block rpc", {
					{"author", {DataType::String} },
					{"extraData", {DataType::String} },
					{"gasLimit", {DataType::String} },
					{"gasUsed", {DataType::String} },
					{"hash", {DataType::String} },
					{"logsBloom", {DataType::String} },
					{"miner", {DataType::String} },
					{"number", {DataType::String} },
					{"parentHash", {DataType::String} },
					{"receiptsRoot", {DataType::String} },
					{"sha3Uncles", {DataType::String} },
					{"size", {DataType::String} },
					{"stateRoot", {DataType::String} },
					{"timestamp", {DataType::String} },
					{"totalDifficulty", {DataType::String} },
					{"transactions", {DataType::Array} },
					{"transactionsRoot", {DataType::String} },
					{"uncles", {DataType::Array} }
			});
            for (auto const& trObj : m_data.at("transactions").getSubObjects())
            {
                requireJsonFields(trObj, "block rpc",
                    {{"blockHash", {DataType::String}}, {"blockNumber", {DataType::String}},
                        {"from", {DataType::String}}, {"gas", {DataType::String}},
                        {"gasPrice", {DataType::String}}, {"hash", {DataType::String}},
                        {"input", {DataType::String}}, {"nonce", {DataType::String}},
                        {"to", {DataType::String}}, {"transactionIndex", {DataType::String}},
                        {"value", {DataType::String}}});
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
            header.appendList(16);
            header << m_data.at("parentHash").asString();
            header << m_data.at("sha3Uncles").asString();
            header << m_data.at("author").asString();
            header << m_data.at("stateRoot").asString();
            header << m_data.at("transactionsRoot").asString();
            header << m_data.at("receiptsRoot").asString();
            header << m_data.at("logsBloom").asString();
            header << m_data.at("totalDifficulty").asString();
            header << m_data.at("number").asString();
            header << m_data.at("gasLimit").asString();
            header << m_data.at("gasUsed").asString();
            header << m_data.at("timestamp").asString();
            header << m_data.at("extraData").asString();
            // mixHash
            // nonce
            stream.appendRaw(header.out());

            RLPStream transactionList(1);
            RLPStream transactionRLP(9);

            DataObject transaction = m_data.at("transactions").getSubObjects().at(0);
            transactionRLP << transaction.at("nonce").asString();
            transactionRLP << transaction.at("gasPrice").asString();
            transactionRLP << transaction.at("gas").asString();
            transactionRLP << transaction.at("to").asString();
            transactionRLP << transaction.at("value").asString();
            transactionRLP << transaction.at("input").asString();
            // v
            // R
            // S

            transactionList.appendRaw(transactionRLP.out());
            stream.appendRaw(transactionList.out());
            stream.appendRaw(RLPStream(0).out());  // empty uncle list

            return "";
        }
    };
}

