#include <retesteth/configs/Options.h>
using namespace std;
using namespace dataobject;

namespace retesteth::options
{
const string default_EIP158ToByzantiumAt5_config = R"({
    "params" : {
        "homesteadForkBlock" : "0x00",
        "EIP150ForkBlock" : "0x00",
        "EIP158ForkBlock" : "0x00",
        "byzantiumForkBlock" : "0x05"
    },
    "accounts" : {
    }
})";

const string t8ntool_EIP158ToByzantiumAt5_config = R"({
    "params" : {
        "fork" : "EIP158ToByzantiumAt5",
        "byzantiumForkBlock" : "0x05",
        "homesteadForkBlock" : "0x00"
    },
    "accounts" : {
    }
})";

genEIP158ToByzantiumCfg::genEIP158ToByzantiumCfg()
{
    spDataObject obj;
    (*obj)["path"] = "besu/genesis/EIP158ToByzantiumAt5.json";
    (*obj)["content"] = default_EIP158ToByzantiumAt5_config;
    map_configs.addArrayObject(obj);

    spDataObject obj2;
    (*obj2)["path"] = "default/genesis/EIP158ToByzantiumAt5.json";
    (*obj2)["content"] = t8ntool_EIP158ToByzantiumAt5_config;
    map_configs.addArrayObject(obj2);
}
}  // namespace retesteth::options
