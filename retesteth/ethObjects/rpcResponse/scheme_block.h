#pragma once
#include "../object.h"



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
		}

		int getTransactionCount() const { return m_data.at("transactions").getSubObjects().size(); }
	};
}

