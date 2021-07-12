#include <retesteth/configs/Options.h>
#include <string>
using namespace std;
using namespace dataobject;

const string default_ByzantiumToConstantinopleFixAt5_config = R"({
    "params" : {
      "homesteadForkBlock" : "0x00",
      "EIP150ForkBlock" : "0x00",
      "EIP158ForkBlock" : "0x00",
      "byzantiumForkBlock" : "0x00",
      "constantinopleForkBlock" : "0x05",
      "constantinopleFixForkBlock" : "0x05"
    },
    "accounts" : {
    }
})";

const string t8ntool_ByzantiumToConstantinopleFixAt5_config = R"({
    "params" : {
        "fork" : "ByzantiumToConstantinopleFixAt5",
        "constantinopleForkBlock" : "0x05",
        "byzantiumForkBlock" : "0x00",
        "homesteadForkBlock" : "0x00"
    },
    "accounts" : {
    }
})";

genByzantiumToConstantinopleFixCfg::genByzantiumToConstantinopleFixCfg()
{
    spDataObject obj(new DataObject());
    (*obj)["path"] = "besu/genesis/ByzantiumToConstantinopleFixAt5.json";
    (*obj)["content"] = default_ByzantiumToConstantinopleFixAt5_config;
    map_configs.addArrayObject(obj);

    spDataObject obj2(new DataObject());
    (*obj2)["path"] = "default/genesis/ByzantiumToConstantinopleFixAt5.json";
    (*obj2)["content"] = t8ntool_ByzantiumToConstantinopleFixAt5_config;
    map_configs.addArrayObject(obj2);
}
