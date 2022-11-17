#include <retesteth/configs/Options.h>
using namespace std;
using namespace dataobject;

namespace retesteth::options
{
const string rpc_EIP150_config = R"({
    "params" : {
       "homesteadForkBlock" : "0x00",
       "EIP150ForkBlock" : "0x00",
       "chainID" : "0x01"
    },
    "accounts" : {
    }
})";

// "homesteadForkBlock" for difficulty verification warning
const string etc_EIP150_config = R"({
    "params" : {
        "fork" : "EIP150",
        "homesteadForkBlock" : "0x00"
    },
    "accounts" : {
    }
})";

genEIP150CfgETC::genEIP150CfgETC()
{
    spDataObject obj;
    (*obj)["path"] = "besuetc/genesis/EIP150.json";
    (*obj)["content"] = rpc_EIP150_config;
    map_configs.addArrayObject(obj);

    spDataObject obj2;
    (*obj2)["path"] = "etc/genesis/EIP150.json";
    (*obj2)["content"] = etc_EIP150_config;
    map_configs.addArrayObject(obj2);
}
}  // namespace retesteth::options
