#include <retesteth/configs/Options.h>
using namespace std;
using namespace dataobject;

namespace retesteth::options
{
const string rpc_Magneto_config = R"({
    "params" : {
        "homesteadForkBlock" : "0x00",
        "EIP150ForkBlock" : "0x00",
        "EIP158ForkBlock" : "0x00",
        "atlantisBlock" : "0x00",
        "aghartaBlock" : "0x00",
        "phoenixBlock" : "0x00",
        "magnetoBlock" : "0x00",
        "chainID" : "0x3d"
    },
    "accounts" : {
    }
})";

const string etc_Magneto_config = R"({
    "params" : {
        "fork" : "ETC_Magneto",
        "chainID" : "0x3d"
    },
    "accounts" : {
    }
})";

genMagnetoCfgETC::genMagnetoCfgETC()
{
    spDataObject obj;
    (*obj)["path"] = "besuetc/genesis/ETC_Magneto.json";
    (*obj)["content"] = rpc_Magneto_config;
    map_configs.addArrayObject(obj);

    spDataObject obj2;
    (*obj2)["path"] = "etc/genesis/ETC_Magneto.json";
    (*obj2)["content"] = etc_Magneto_config;
    map_configs.addArrayObject(obj2);
}
}  // namespace retesteth::options
