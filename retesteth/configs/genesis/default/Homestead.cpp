#include <retesteth/configs/Options.h>
#include <string>
using namespace std;
using namespace dataobject;

const string default_Homestead_config = R"({
    "params" : {
        "homesteadForkBlock" : "0x00"
    },
    "accounts" : {
    }
})";

const string t8ntool_Homestead_config = R"({
    "params" : {
        "fork" : "Homestead",
        "homesteadForkBlock" : "0x00"
    },
    "accounts" : {
    }
})";

genHomesteadCfg::genHomesteadCfg()
{
    spDataObject obj(new DataObject());
    (*obj)["path"] = "besu/genesis/Homestead.json";
    (*obj)["content"] = default_Homestead_config;
    map_configs.addArrayObject(obj);

    spDataObject obj2(new DataObject());
    (*obj2)["path"] = "default/genesis/Homestead.json";
    (*obj2)["content"] = t8ntool_Homestead_config;
    map_configs.addArrayObject(obj2);
}
