#include <retesteth/configs/Options.h>
using namespace std;
using namespace dataobject;

namespace retesteth::options
{
genFrontierCfg::genFrontierCfg()
{

const string t8ntool_Frontier_config = R"({
    "params" : {
        "fork" : "Frontier",
        "chainID" : "0x01"
    },
    "accounts" : {
    }
})";

    spDataObject obj2;
    (*obj2)["path"] = "default/genesis/Frontier.json";
    (*obj2)["content"] = t8ntool_Frontier_config;
    map_configs.addArrayObject(obj2);
}
}  // namespace retesteth::options
