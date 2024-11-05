#include <retesteth/configs/Options.h>
using namespace std;
using namespace dataobject;

namespace retesteth::options
{
genOsakaCfg::genOsakaCfg()
{

const string t8ntool_Osaka_config = R"({
    "params" : {
        "fork" : "Osaka",
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
        (*obj)["path"] = "default/genesis/Osaka.json";
        (*obj)["content"] = t8ntool_Osaka_config;
        map_configs.addArrayObject(obj);
    }
}
}  // namespace retesteth::options
