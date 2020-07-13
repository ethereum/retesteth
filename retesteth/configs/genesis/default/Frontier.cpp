#include <retesteth/configs/Options.h>
#include <string>
using namespace std;
using namespace dataobject;

const string default_Frontier_config = R"({
    "params" : {
    },
    "accounts" : {
    }
})";

const string t8ntool_Frontier_config = R"({
    "params" : {
        "fork" : "Frontier"
    },
    "accounts" : {
    }
})";

genFrontierCfg::genFrontierCfg()
{
    DataObject obj;
    obj["path"] = "default/genesis/Frontier.json";
    obj["content"] = default_Frontier_config;
    map_configs.addArrayObject(obj);

    DataObject obj2;
    obj2["path"] = "t8ntool/genesis/Frontier.json";
    obj2["content"] = t8ntool_Frontier_config;
    map_configs.addArrayObject(obj2);
}

