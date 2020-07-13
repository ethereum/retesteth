#include <retesteth/configs/Options.h>
#include <string>
using namespace std;
using namespace dataobject;

const string default_HomesteadToDaoAt5_config = R"({
    "params" : {
        "homesteadForkBlock" : "0x00",
        "daoHardforkBlock" : "0x05"
    },
    "accounts" : {
    }
})";

const string t8ntool_HomesteadToDaoAt5_config = R"({
    "params" : {
        "fork" : "HomesteadToDaoAt5",
        "homesteadForkBlock" : "0x00"
    },
    "accounts" : {
    }
})";

genHomesteadToDaoCfg::genHomesteadToDaoCfg()
{
    DataObject obj;
    obj["path"] = "default/genesis/HomesteadToDaoAt5.json";
    obj["content"] = default_HomesteadToDaoAt5_config;
    map_configs.addArrayObject(obj);

    DataObject obj2;
    obj2["path"] = "t8ntool/genesis/HomesteadToDaoAt5.json";
    obj2["content"] = t8ntool_HomesteadToDaoAt5_config;
    map_configs.addArrayObject(obj2);
}
