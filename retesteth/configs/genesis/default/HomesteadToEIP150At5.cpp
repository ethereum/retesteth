#include <retesteth/configs/Options.h>
using namespace std;
using namespace dataobject;

namespace retesteth::options
{
const string default_HomesteadToEIP150At5_config = R"({
    "params" : {
        "homesteadForkBlock" : "0x00",
        "EIP150ForkBlock" : "0x05"
    },
    "accounts" : {
    }
})";

const string t8ntool_HomesteadToEIP150At5_config = R"({
    "params" : {
        "fork" : "HomesteadToEIP150At5",
        "homesteadForkBlock" : "0x00"
    },
    "accounts" : {
    }
})";

genHomesteadToEIP150Cfg::genHomesteadToEIP150Cfg()
{
    spDataObject obj;
    (*obj)["path"] = "besu/genesis/HomesteadToEIP150At5.json";
    (*obj)["content"] = default_HomesteadToEIP150At5_config;
    map_configs.addArrayObject(obj);

    spDataObject obj2;
    (*obj2)["path"] = "default/genesis/HomesteadToEIP150At5.json";
    (*obj2)["content"] = t8ntool_HomesteadToEIP150At5_config;
    map_configs.addArrayObject(obj2);
}
}  // namespace retesteth::options
