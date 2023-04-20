#include <retesteth/configs/Options.h>
using namespace std;
using namespace dataobject;

namespace retesteth::options
{

genGrayGlacierCfg::genGrayGlacierCfg()
{

const string t8ntool_GrayGlacier_config = R"({
    "params" : {
        "fork" : "GrayGlacier",
        "constantinopleForkBlock" : "0x00",
        "byzantiumForkBlock" : "0x00",
        "homesteadForkBlock" : "0x00",
        "chainID" : "0x01"
    },
    "accounts" : {
    }
})";


    spDataObject obj2(new DataObject());
    (*obj2)["path"] = "default/genesis/GrayGlacier.json";
    (*obj2)["content"] = t8ntool_GrayGlacier_config;
    map_configs.addArrayObject(obj2);
}
}  // namespace retesteth::options
