#include <retesteth/configs/Options.h>
using namespace std;
using namespace dataobject;

namespace retesteth::options
{

genEIP158Cfg::genEIP158Cfg()
{

const string t8ntool_EIP158_config = R"({
    "params" : {
        "fork" : "EIP158",
        "homesteadForkBlock" : "0x00",
        "chainID" : "0x01"
    },
    "accounts" : {
    }
})";

    spDataObject obj2;
    (*obj2)["path"] = "default/genesis/EIP158.json";
    (*obj2)["content"] = t8ntool_EIP158_config;
    map_configs.addArrayObject(obj2);
}
}  // namespace retesteth::options
