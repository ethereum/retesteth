#include <retesteth/configs/Options.h>
using namespace std;
using namespace dataobject;

namespace retesteth::options
{
const string t8ntool_Homestead_config = R"({
    "params" : {
        "fork" : "Homestead",
        "homesteadForkBlock" : "0x00",
        "chainID" : "0x01"
    },
    "accounts" : {
    }
})";

genHomesteadCfg::genHomesteadCfg()
{
    spDataObject obj2;
    (*obj2)["path"] = "default/genesis/Homestead.json";
    (*obj2)["content"] = t8ntool_Homestead_config;
    map_configs.addArrayObject(obj2);
}
}  // namespace retesteth::options
