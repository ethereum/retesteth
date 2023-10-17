#include <retesteth/configs/Options.h>
using namespace std;
using namespace dataobject;

namespace retesteth::options
{

genFrontierToHomesteadCfg::genFrontierToHomesteadCfg()
{

const string t8ntool_FrontierToHomesteadAt5_config = R"({
    "params" : {
        "fork" : "FrontierToHomesteadAt5",
        "homesteadForkBlock" : "0x05",
        "chainID" : "0x01"
    },
    "accounts" : {
    }
})";

    spDataObject obj2;
    (*obj2)["path"] = "default/genesis/FrontierToHomesteadAt5.json";
    (*obj2)["content"] = t8ntool_FrontierToHomesteadAt5_config;
    map_configs.addArrayObject(obj2);
}
}  // namespace retesteth::options
