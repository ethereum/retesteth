#include <retesteth/configs/Options.h>
using namespace std;
using namespace dataobject;

namespace retesteth::options
{
const string rpc_Byzantium_config = R"({
    "params" : {
        "homesteadForkBlock" : "0x00",
        "EIP150ForkBlock" : "0x00",
        "EIP158ForkBlock" : "0x00",
        "atlantisBlock" : "0x00",
        "chainID" : "0x3d"
    },
    "accounts" : {
    }
})";

const string etc_Atlantis_config = R"({
    "params" : {
        "fork" : "ETC_Atlantis",
        "chainID" : "0x3d"
    },
    "accounts" : {
    }
})";

genAtlantisCfgETC::genAtlantisCfgETC()
{
    spDataObject obj;
    (*obj)["path"] = "besuetc/genesis/ETC_Atlantis.json";
    (*obj)["content"] = rpc_Byzantium_config;
    map_configs.addArrayObject(obj);

    spDataObject obj2;
    (*obj2)["path"] = "etc/genesis/ETC_Atlantis.json";
    (*obj2)["content"] = etc_Atlantis_config;
    map_configs.addArrayObject(obj2);
}
}  // namespace retesteth::options
