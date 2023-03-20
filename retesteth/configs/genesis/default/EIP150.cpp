#include <retesteth/configs/Options.h>
using namespace std;
using namespace dataobject;

namespace retesteth::options
{
const string t8ntool_EIP150_config = R"({
    "params" : {
        "fork" : "EIP150",
        "homesteadForkBlock" : "0x00",
        "chainID" : "0x01"
    },
    "accounts" : {
    }
})";

genEIP150Cfg::genEIP150Cfg()
{
    spDataObject obj2;
    (*obj2)["path"] = "default/genesis/EIP150.json";
    (*obj2)["content"] = t8ntool_EIP150_config;
    map_configs.addArrayObject(obj2);
}
}  // namespace retesteth::options
