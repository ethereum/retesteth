#pragma once
#include <testeth/DataObject.h>
#include <testeth/TestHelper.h>
#include <testeth/ethObjects/state.h>

using namespace dev;

namespace test {
	class genesis
	{
		public:
		genesis(DataObject const& _env, state const& _state):
			m_info(_env), m_state(_state)
		{
			test::requireJsonFields(_env, "env", {
				{"currentCoinbase", DataType::String},
				{"currentDifficulty", DataType::String},
				{"currentGasLimit", DataType::String},
				{"currentNumber", DataType::String},
				{"currentTimestamp", DataType::String},
				{"previousHash", DataType::String},
			});
		}

		private:
		DataObject m_info;
		state m_state;
	};
}

