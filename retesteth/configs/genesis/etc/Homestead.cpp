#include <retesteth/configs/Options.h>
using namespace std;
using namespace dataobject;

namespace retesteth::options
{
const string rpc_Homestead_config = R"({
    "params" : {
        "homesteadForkBlock" : "0x00",
        "chainID" : "0x01"
    },
    "accounts" : {
    }
})";

// "homesteadForkBlock" for difficulty verification warning
const string etc_Homestead_config = R"({
    "params" : {
        "fork" : "Homestead",
        "homesteadForkBlock" : "0x00"
    },
    "accounts" : {
    }
})";

genHomesteadCfgETC::genHomesteadCfgETC()
{
    spDataObject obj;
    (*obj)["path"] = "besuetc/genesis/Homestead.json";
    (*obj)["content"] = rpc_Homestead_config;
    map_configs.addArrayObject(obj);

    spDataObject obj2;
    (*obj2)["path"] = "etc/genesis/Homestead.json";
    (*obj2)["content"] = etc_Homestead_config;
    map_configs.addArrayObject(obj2);
}
}  // namespace retesteth::options
