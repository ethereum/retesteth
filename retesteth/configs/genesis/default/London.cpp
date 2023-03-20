#include <retesteth/configs/Options.h>
using namespace std;
using namespace dataobject;

namespace retesteth::options
{
const string t8ntool_London_config = R"({
    "params" : {
        "fork" : "London",
        "constantinopleForkBlock" : "0x00",
        "byzantiumForkBlock" : "0x00",
        "homesteadForkBlock" : "0x00",
        "chainID" : "0x01"
    },
    "accounts" : {
    }
})";

genLondonCfg::genLondonCfg()
{
    spDataObject obj2;
    (*obj2)["path"] = "default/genesis/London.json";
    (*obj2)["content"] = t8ntool_London_config;
    map_configs.addArrayObject(obj2);
}
}  // namespace retesteth::options
