#pragma once
#include "../object.h"

using namespace  test;

namespace test {
    class scheme_env: public object
    {
        public:
        scheme_env(DataObject const& _env):
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
