#pragma once
#include <testeth/DataObject.h>
#include <testeth/TestHelper.h>
#include <testeth/ethObjects/state.h>
#include <testeth/ethObjects/object.h>

using namespace dev;

namespace test {
	class genesis: public object
	{
		public:
		genesis(DataObject const& _env, state const& _state):
			object(_env), m_state(_state)
		{
			test::requireJsonFields(_env, "env", {
				{"currentCoinbase", DataType::String},
				{"currentDifficulty", DataType::String},
				{"currentGasLimit", DataType::String},
				{"currentNumber", DataType::String},
				{"currentTimestamp", DataType::String},
				{"previousHash", DataType::String},
			});

			// Convert from state test format to unified json rpc names
			m_data.renameKey("currentCoinbase", "coinbase");
			m_data.renameKey("currentDifficulty", "difficulty");
			m_data.renameKey("currentGasLimit", "gasLimit");
			m_data.renameKey("currentNumber", "number");
			m_data.renameKey("currentTimestamp", "timestamp");
			m_data.renameKey("previousHash", "parentHash");
		}

		private:
		state m_state;
	};
}

