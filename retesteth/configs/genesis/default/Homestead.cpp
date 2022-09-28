#include <retesteth/configs/Options.h>
using namespace std;
using namespace dataobject;

namespace retesteth::options
{
const string default_Homestead_config = R"({
    "params" : {
        "homesteadForkBlock" : "0x00"
    },
    "accounts" : {
    }
})";

const string t8ntool_Homestead_config = R"({
    "params" : {
        "fork" : "Homestead",
        "homesteadForkBlock" : "0x00"
    },
    "accounts" : {
    }
})";

genHomesteadCfg::genHomesteadCfg()
{
    spDataObject obj;
    (*obj)["path"] = "besu/genesis/Homestead.json";
    (*obj)["content"] = default_Homestead_config;
    map_configs.addArrayObject(obj);

    spDataObject obj2;
    (*obj2)["path"] = "default/genesis/Homestead.json";
    (*obj2)["content"] = t8ntool_Homestead_config;
    map_configs.addArrayObject(obj2);
}
}  // namespace retesteth::options
