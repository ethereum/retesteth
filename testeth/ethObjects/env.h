#pragma once
#include <testeth/DataObject.h>
#include <testeth/TestHelper.h>
#include <testeth/ethObjects/object.h>

using namespace dev;

namespace test {
    class env: public object
	{
		public:
        env(DataObject const& _env):
            object(_env)
		{
			test::requireJsonFields(_env, "env", {
				{"currentCoinbase", {DataType::String} },
				{"currentDifficulty", {DataType::String} },
				{"currentGasLimit", {DataType::String} },
				{"currentNumber", {DataType::String} },
				{"currentTimestamp", {DataType::String} },
				{"previousHash", {DataType::String} },
			});

			makeAllFieldsHex(m_data);
		}

		DataObject getDataForRPC() const
		{
			DataObject data;
			data.replace(m_data);
			// Convert from state test format to unified json rpc names
			data.renameKey("currentCoinbase", "coinbase");
			data.renameKey("currentDifficulty", "difficulty");
			data.renameKey("currentGasLimit", "gasLimit");
			data.renameKey("currentNumber", "number");
			data.renameKey("currentTimestamp", "timestamp");
			data.renameKey("previousHash", "parentHash");
			return data;
		}
	};
}

