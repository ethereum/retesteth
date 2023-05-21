#include <retesteth/configs/Options.h>
using namespace std;
using namespace dataobject;

namespace retesteth::options
{
genPhoenixCfgETC::genPhoenixCfgETC()
{

const string rpc_Phoenix_config = R"({
    "params" : {
        "homesteadForkBlock" : "0x00",
        "EIP150ForkBlock" : "0x00",
        "EIP158ForkBlock" : "0x00",
        "atlantisBlock" : "0x00",
        "aghartaBlock" : "0x00",
        "phoenixBlock" : "0x00",
        "chainID" : "0x3d"
    },
    "accounts" : {
    }
})";

const string etc_Phoenix_config = R"({
    "params" : {
        "fork" : "ETC_Phoenix",
        "chainID" : "0x3d"
    },
    "accounts" : {
    }
})";

    spDataObject obj;
    (*obj)["path"] = "besuetc/genesis/ETC_Phoenix.json";
    (*obj)["content"] = rpc_Phoenix_config;
    map_configs.addArrayObject(obj);

    spDataObject obj2;
    (*obj2)["path"] = "etc/genesis/ETC_Phoenix.json";
    (*obj2)["content"] = etc_Phoenix_config;
    map_configs.addArrayObject(obj2);
}
}  // namespace retesteth::options
