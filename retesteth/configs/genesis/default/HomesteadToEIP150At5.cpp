#include <retesteth/configs/Options.h>
using namespace std;
using namespace dataobject;

namespace retesteth::options
{
genHomesteadToEIP150Cfg::genHomesteadToEIP150Cfg()
{

const string t8ntool_HomesteadToEIP150At5_config = R"({
    "params" : {
        "fork" : "HomesteadToEIP150At5",
        "homesteadForkBlock" : "0x00",
        "chainID" : "0x01"
    },
    "accounts" : {
    }
})";

    spDataObject obj2;
    (*obj2)["path"] = "default/genesis/HomesteadToEIP150At5.json";
    (*obj2)["content"] = t8ntool_HomesteadToEIP150At5_config;
    map_configs.addArrayObject(obj2);
}
}  // namespace retesteth::options
