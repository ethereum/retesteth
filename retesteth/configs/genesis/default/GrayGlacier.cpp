#include <retesteth/configs/Options.h>
#include <string>
using namespace std;
using namespace dataobject;

namespace retesteth::options
{
const string default_GrayGlacier_config = R"({
    "params" : {
       "homesteadForkBlock" : "0x00",
       "EIP150ForkBlock" : "0x00",
       "EIP158ForkBlock" : "0x00",
       "byzantiumForkBlock" : "0x00",
       "constantinopleForkBlock" : "0x00",
       "constantinopleFixForkBlock" : "0x00",
       "istanbulForkBlock" : "0x00",
       "berlinForkBlock" : "0x00",
       "londonForkBlock" : "0x00",
       "arrowGlacierForkBlock" : "0x00",
       "grayGlacierForkBlock" : "0x00",
       "chainID" : "0x01"
    },
    "accounts" : {
    }
})";

const string t8ntool_GrayGlacier_config = R"({
    "params" : {
        "fork" : "GrayGlacier",
        "constantinopleForkBlock" : "0x00",
        "byzantiumForkBlock" : "0x00",
        "homesteadForkBlock" : "0x00"
    },
    "accounts" : {
    }
})";

genGrayGlacierCfg::genGrayGlacierCfg()
{
    spDataObject obj(new DataObject());
    (*obj)["path"] = "besu/genesis/GrayGlacier.json";
    (*obj)["content"] = default_GrayGlacier_config;
    map_configs.addArrayObject(obj);

    spDataObject obj2(new DataObject());
    (*obj2)["path"] = "default/genesis/GrayGlacier.json";
    (*obj2)["content"] = t8ntool_GrayGlacier_config;
    map_configs.addArrayObject(obj2);
}
}  // namespace retesteth::options
