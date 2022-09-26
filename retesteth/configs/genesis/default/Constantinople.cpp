#include <retesteth/configs/Options.h>
#include <string>
using namespace std;
using namespace dataobject;

namespace retesteth::options
{
const string default_Constantinople_config = R"({
    "params" : {
       "homesteadForkBlock" : "0x00",
       "EIP150ForkBlock" : "0x00",
       "EIP158ForkBlock" : "0x00",
       "byzantiumForkBlock" : "0x00",
       "constantinopleForkBlock" : "0x00"
    },
    "accounts" : {
    }
})";

const string t8ntool_Constantinople_config = R"({
    "params" : {
        "fork" : "Constantinople",
        "constantinopleForkBlock" : "0x00",
        "byzantiumForkBlock" : "0x00",
        "homesteadForkBlock" : "0x00"
    },
    "accounts" : {
    }
})";

genConstantinopleCfg::genConstantinopleCfg()
{
    spDataObject obj;
    (*obj)["path"] = "besu/genesis/Constantinople.json";
    (*obj)["content"] = default_Constantinople_config;
    map_configs.addArrayObject(obj);

    spDataObject obj2;
    (*obj2)["path"] = "default/genesis/Constantinople.json";
    (*obj2)["content"] = t8ntool_Constantinople_config;
    map_configs.addArrayObject(obj2);
}
}  // namespace retesteth::options
