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
            data["author"] = m_data.at("currentCoinbase");
            data["difficulty"] = m_data.at("currentDifficulty");
            data["gasLimit"] = m_data.at("currentGasLimit");
            data["nonce"] = "0x00";
            data["extraData"] = "0x00";
            // this is genesis block timestamp
            // the actual timestamp of a block is set in the env section
            data["timestamp"] = "0x00";

            // Convert from state test format to unified json rpc names
            //data.renameKey("currentCoinbase", "coinbase");
            //data.renameKey("currentDifficulty", "difficulty");
            //data.renameKey("currentGasLimit", "gasLimit");
            //data.renameKey("currentNumber", "number");
            //data.renameKey("currentTimestamp", "timestamp");
            //data.renameKey("previousHash", "parentHash");

            /*
            "author": "0000000000000010000000000000000000000000",
            "timestamp": "0x00",
            "parentHash": "0x0000000000000000000000000000000000000000000000000000000000000000",
            "extraData": "0x",
            "gasLimit": "0x1000000000000"
            */

            return data;
        }
    };
}
