#include <retesteth/configs/Options.h>
#include <string>
using namespace std;
using namespace dataobject;

const string default_Shanghai_config = R"({
    "params" : {
       "homesteadForkBlock" : "0x00",
       "EIP150ForkBlock" : "0x00",
       "EIP158ForkBlock" : "0x00",
       "byzantiumForkBlock" : "0x00",
       "constantinopleForkBlock" : "0x00",
       "constantinopleFixForkBlock" : "0x00",
       "istanbulForkBlock" : "0x00",
       "berlinForkBlock" : "0x00",
       "londonForkBlock" : "0x00",
       "shanghaiForkBlock" : "0x00",
       "terminalTotalDifficulty" : "0x00",
       "chainID" : "0x01"
    },
    "accounts" : {
    }
})";

const string t8ntool_Shanghai_config = R"({
    "params" : {
        "fork" : "Shanghai",
        "terminalTotalDifficulty" : "0x00",
        "constantinopleForkBlock" : "0x00",
        "byzantiumForkBlock" : "0x00",
        "homesteadForkBlock" : "0x00"
    },
    "accounts" : {
    }
})";

genShanghaiCfg::genShanghaiCfg()
{
    {
        spDataObject obj;
        (*obj)["path"] = "besu/genesis/Shanghai.json";
        (*obj)["content"] = default_Shanghai_config;
        map_configs.addArrayObject(obj);
    }

    {
        spDataObject obj;
        (*obj)["path"] = "default/genesis/Shanghai.json";
        (*obj)["content"] = t8ntool_Shanghai_config;
        map_configs.addArrayObject(obj);
    }
}
