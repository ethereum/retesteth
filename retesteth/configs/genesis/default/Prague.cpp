#include <retesteth/configs/Options.h>
using namespace std;
using namespace dataobject;

namespace retesteth::options
{
genPragueCfg::genPragueCfg()
{

const string t8ntool_Prague_config = R"({
    "params" : {
        "fork" : "Prague",
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
        (*obj)["path"] = "default/genesis/Prague.json";
        (*obj)["content"] = t8ntool_Prague_config;
        map_configs.addArrayObject(obj);
    }
}
}  // namespace retesteth::options
