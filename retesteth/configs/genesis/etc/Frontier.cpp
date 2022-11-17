#include <retesteth/configs/Options.h>
using namespace std;
using namespace dataobject;

namespace retesteth::options
{
const string etc_Frontier_config = R"({
    "params" : {
        "fork" : "Frontier",
        "chainID" : "0x01"
    },
    "accounts" : {
    }
})";

const string rpc_Frontier_config = R"({
    "params" : {
    },
    "accounts" : {
    }
})";


genFrontierCfgETC::genFrontierCfgETC()
{
    spDataObject obj;
    (*obj)["path"] = "besuetc/genesis/Frontier.json";
    (*obj)["content"] = rpc_Frontier_config;
    map_configs.addArrayObject(obj);

    spDataObject obj2;
    (*obj2)["path"] = "etc/genesis/Frontier.json";
    (*obj2)["content"] = etc_Frontier_config;
    map_configs.addArrayObject(obj2);
}
}  // namespace retesteth::options
