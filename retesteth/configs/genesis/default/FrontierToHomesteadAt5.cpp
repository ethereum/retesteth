#include <retesteth/configs/Options.h>
using namespace std;
using namespace dataobject;

namespace retesteth::options
{
const string default_FrontierToHomesteadAt5_config = R"({
    "params" : {
        "homesteadForkBlock" : "0x05"
    },
    "accounts" : {
    }
})";

const string t8ntool_FrontierToHomesteadAt5_config = R"({
    "params" : {
        "fork" : "FrontierToHomesteadAt5",
        "homesteadForkBlock" : "0x05"
    },
    "accounts" : {
    }
})";

genFrontierToHomesteadCfg::genFrontierToHomesteadCfg()
{
    spDataObject obj;
    (*obj)["path"] = "besu/genesis/FrontierToHomesteadAt5.json";
    (*obj)["content"] = default_FrontierToHomesteadAt5_config;
    map_configs.addArrayObject(obj);

    spDataObject obj2;
    (*obj2)["path"] = "default/genesis/FrontierToHomesteadAt5.json";
    (*obj2)["content"] = t8ntool_FrontierToHomesteadAt5_config;
    map_configs.addArrayObject(obj2);
}
}  // namespace retesteth::options
