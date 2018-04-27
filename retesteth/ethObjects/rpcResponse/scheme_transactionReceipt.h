#pragma once
#include "../object.h"

#include <retesteth/TestHelper.h>
#include <libdevcore/RLP.h>
#include <libdevcore/SHA3.h>
#include <libdevcore/Address.h>

namespace test {
	class scheme_transactionReceipt: public object
	{
		public:
		scheme_transactionReceipt(DataObject const& _receipt):
			object(_receipt)
		{
			std::string statusField = "status";
			if (_receipt.count("stateRoot"))
				statusField = "stateRoot";

			requireJsonFields(_receipt, "transactionReceipt", {
					{"blockHash", {DataType::String} },
					{"blockNumber", {DataType::Integer} },
					{"contractAddress", {DataType::String} },
					{"cumulativeGasUsed", {DataType::String} },
					{"gasUsed", {DataType::String} },
					{"logs", {DataType::Array} },
					{"logsBloom", {DataType::String} },
					{statusField, {DataType::String} },
					{"transactionHash", {DataType::String} },
					{"transactionIndex", {DataType::Integer} }
				});

			for (auto const& log: m_data.at("logs").getSubObjects())
				m_logs.push_back(logs(log));

		}

		std::string getLogHash()
		{
			dev::RLPStream s;
			s.appendList(m_logs.size());
			for (logs const& l : m_logs)
				l.streamRLP(s);
			return dev::toHexPrefixed(dev::sha3(s.out()));
		}

		private:
		class logs : public object
		{
			public:
			logs(DataObject const& _logs):
				object(_logs)
			{
				requireJsonFields(_logs, "transactionReceipt_logs", {
						{"address", {DataType::String} },
						{"blockHash", {DataType::String} },
						{"blockNumber", {DataType::Integer} },
						{"data", {DataType::String} },
						{"logIndex", {DataType::Integer} },
						{"polarity", {DataType::Integer} },
						{"topics", {DataType::Array} },
						{"transactionHash", {DataType::String} },
						{"transactionIndex", {DataType::Integer} },
						{"type", {DataType::String} }
					});
			}

			void streamRLP(dev::RLPStream& _rlp) const
			{
				std::vector<dev::h256> topics;
				for (auto const& topic: m_data.at("topics").getSubObjects())
					topics.push_back(dev::h256(topic.asString()));

				_rlp.appendList(3) << dev::Address(m_data.at("address").asString())
								<< topics << dev::fromHex(m_data.at("data").asString());
			}
		};

		std::vector<logs> m_logs;
	};
}



