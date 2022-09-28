#include <retesteth/configs/Options.h>
using namespace std;
using namespace dataobject;

namespace retesteth::options
{
const string default_Byzantium_config = R"({
    "params" : {
        "homesteadForkBlock" : "0x00",
        "EIP150ForkBlock" : "0x00",
        "EIP158ForkBlock" : "0x00",
        "byzantiumForkBlock" : "0x00"
    },
    "accounts" : {
    }
})";

const string t8ntool_Byzantium_config = R"({
    "params" : {
        "fork" : "Byzantium",
        "byzantiumForkBlock" : "0x00",
        "homesteadForkBlock" : "0x00"
    },
    "accounts" : {
    }
})";

genByzantiumCfg::genByzantiumCfg()
{
    spDataObject obj;
    (*obj)["path"] = "besu/genesis/Byzantium.json";
    (*obj)["content"] = default_Byzantium_config;
    map_configs.addArrayObject(obj);

    spDataObject obj2;
    (*obj2)["path"] = "default/genesis/Byzantium.json";
    (*obj2)["content"] = t8ntool_Byzantium_config;
    map_configs.addArrayObject(obj2);
}
}  // namespace retesteth::options
