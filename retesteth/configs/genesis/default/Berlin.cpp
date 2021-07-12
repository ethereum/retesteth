#include <retesteth/configs/Options.h>
#include <string>
using namespace std;
using namespace dataobject;

const string default_Berlin_config = R"({
    "params" : {
       "homesteadForkBlock" : "0x00",
       "EIP150ForkBlock" : "0x00",
       "EIP158ForkBlock" : "0x00",
       "byzantiumForkBlock" : "0x00",
       "constantinopleForkBlock" : "0x00",
       "constantinopleFixForkBlock" : "0x00",
       "istanbulForkBlock" : "0x00",
       "berlinForkBlock" : "0x00",
       "chainID" : "0x01"
    },
    "accounts" : {
    }
})";

const string t8ntool_Berlin_config = R"({
    "params" : {
        "fork" : "Berlin",
        "constantinopleForkBlock" : "0x00",
        "byzantiumForkBlock" : "0x00",
        "homesteadForkBlock" : "0x00"
    },
    "accounts" : {
    }
})";

genBerlinCfg::genBerlinCfg()
{
    spDataObject obj (new DataObject());
    (*obj)["path"] = "besu/genesis/Berlin.json";
    (*obj)["content"] = default_Berlin_config;
    map_configs.addArrayObject(obj);

    spDataObject obj2 (new DataObject());
    (*obj2)["path"] = "default/genesis/Berlin.json";
    (*obj2)["content"] = t8ntool_Berlin_config;
    map_configs.addArrayObject(obj2);
}
