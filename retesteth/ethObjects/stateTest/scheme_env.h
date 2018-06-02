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

        std::string getCoinbase() const { return m_data.at("currentCoinbase").asString(); }
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
            data["mixHash"] = "0x00";
            return data;
        }
    };
}
