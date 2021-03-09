#include <retesteth/configs/Options.h>
#include <string>
using namespace std;
using namespace dataobject;

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
    DataObject obj;
    obj["path"] = "besu/genesis/FrontierToHomesteadAt5.json";
    obj["content"] = default_FrontierToHomesteadAt5_config;
    map_configs.addArrayObject(obj);

    DataObject obj2;
    obj2["path"] = "default/genesis/FrontierToHomesteadAt5.json";
    obj2["content"] = t8ntool_FrontierToHomesteadAt5_config;
    map_configs.addArrayObject(obj2);
}
