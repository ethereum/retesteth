#include <retesteth/configs/Options.h>
using namespace std;
using namespace dataobject;

namespace retesteth::options
{
genEIP158ToByzantiumCfg::genEIP158ToByzantiumCfg()
{

const string t8ntool_EIP158ToByzantiumAt5_config = R"({
    "params" : {
        "fork" : "EIP158ToByzantiumAt5",
        "byzantiumForkBlock" : "0x05",
        "homesteadForkBlock" : "0x00",
        "chainID" : "0x01"
    },
    "accounts" : {
    }
})";

    spDataObject obj2;
    (*obj2)["path"] = "default/genesis/EIP158ToByzantiumAt5.json";
    (*obj2)["content"] = t8ntool_EIP158ToByzantiumAt5_config;
    map_configs.addArrayObject(obj2);
}
}  // namespace retesteth::options
