#include <retesteth/configs/Options.h>
#include <string>
using namespace std;
using namespace dataobject;

namespace retesteth::options
{
const string default_ConstantinopleFix_config = R"({
    "params" : {
       "homesteadForkBlock" : "0x00",
       "EIP150ForkBlock" : "0x00",
       "EIP158ForkBlock" : "0x00",
       "byzantiumForkBlock" : "0x00",
       "constantinopleForkBlock" : "0x00",
       "constantinopleFixForkBlock" : "0x00"
    },
    "accounts" : {
    }
})";

const string t8ntool_ConstantinopleFix_config = R"({
    "params" : {
        "fork" : "ConstantinopleFix",
        "constantinopleForkBlock" : "0x00",
        "byzantiumForkBlock" : "0x00",
        "homesteadForkBlock" : "0x00"
    },
    "accounts" : {
    }
})";

genConstantinopleFixCfg::genConstantinopleFixCfg()
{
    spDataObject obj;
    (*obj)["path"] = "besu/genesis/ConstantinopleFix.json";
    (*obj)["content"] = default_ConstantinopleFix_config;
    map_configs.addArrayObject(obj);

    spDataObject obj2;
    (*obj2)["path"] = "default/genesis/ConstantinopleFix.json";
    (*obj2)["content"] = t8ntool_ConstantinopleFix_config;
    map_configs.addArrayObject(obj2);
}
}  // namespace retesteth::options
