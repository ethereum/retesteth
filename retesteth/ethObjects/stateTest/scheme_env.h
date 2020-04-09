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
            test::requireJsonFields(_env, _env.getKey(), {
                {"currentCoinbase", {DataType::String} },
                {"currentDifficulty", {DataType::String} },
                {"currentGasLimit", {DataType::String} },
                {"currentNumber", {DataType::String} },
                {"currentTimestamp", {DataType::String} },
                {"previousHash", {DataType::String} },
            });

            ETH_ERROR_REQUIRE_MESSAGE(dev::u256(_env.atKey("currentGasLimit").asString()) <=
                                          dev::u256("0x7fffffffffffffff"),
                "Env section gasLimit is beyound the limits! ");
            makeAllFieldsHex(m_data);
        }

        std::string getCoinbase() const { return m_data.atKey("currentCoinbase").asString(); }
        DataObject getDataForRPC() const
        {
            DataObject data;
            data["Coinbase"] = m_data.atKey("currentCoinbase");
            data["Difficulty"] = m_data.atKey("currentDifficulty");
            data["GasLimit"] = m_data.atKey("currentGasLimit");
            data["Nonce"] = "0x0000000000000000";
            data["ExtraData"] = "0x00";
            // this is genesis block timestamp
            // the actual timestamp of a block is set in the env section
            data["Timestamp"] = "0x00";
            data["Mixhash"] = "0x0000000000000000000000000000000000000000000000000000000000000000";
            return data;
        }
    };
}
