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
    spDataObject obj(new DataObject());
    (*obj)["path"] = "besu/genesis/Frontier.json";
    (*obj)["content"] = default_Frontier_config;
    map_configs.addArrayObject(obj);

    spDataObject obj2(new DataObject());
    (*obj2)["path"] = "default/genesis/Frontier.json";
    (*obj2)["content"] = t8ntool_Frontier_config;
    map_configs.addArrayObject(obj2);
}

