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

			// Convert from state test format to unified json rpc names
			m_data.renameKey("currentCoinbase", "coinbase");
			m_data.renameKey("currentDifficulty", "difficulty");
			m_data.renameKey("currentGasLimit", "gasLimit");
			m_data.renameKey("currentNumber", "number");
			m_data.renameKey("currentTimestamp", "timestamp");
			m_data.renameKey("previousHash", "parentHash");
            makeAllFieldsHex(m_data);
		}
	};
}

