#include <retesteth/configs/Options.h>
using namespace std;
using namespace dataobject;

namespace retesteth::options
{
genShanghaiCfg::genShanghaiCfg()
{

const string t8ntool_Shanghai_config = R"({
    "params" : {
        "fork" : "Shanghai",
        "terminalTotalDifficulty" : "0x00",
        "constantinopleForkBlock" : "0x00",
        "byzantiumForkBlock" : "0x00",
        "homesteadForkBlock" : "0x00",
        "chainID" : "0x01"
    },
    "accounts" : {
    }
})";

    {
        spDataObject obj;
        (*obj)["path"] = "default/genesis/Shanghai.json";
        (*obj)["content"] = t8ntool_Shanghai_config;
        map_configs.addArrayObject(obj);
    }
}
}  // namespace retesteth::options
