#include <retesteth/configs/Options.h>
using namespace std;
using namespace dataobject;

namespace retesteth::options
{
genByzantiumToConstantinopleFixCfg::genByzantiumToConstantinopleFixCfg()
{

const string t8ntool_ByzantiumToConstantinopleFixAt5_config = R"({
    "params" : {
        "fork" : "ByzantiumToConstantinopleFixAt5",
        "constantinopleForkBlock" : "0x05",
        "byzantiumForkBlock" : "0x00",
        "homesteadForkBlock" : "0x00",
        "chainID" : "0x01"
    },
    "accounts" : {
    }
})";


    spDataObject obj2;
    (*obj2)["path"] = "default/genesis/ByzantiumToConstantinopleFixAt5.json";
    (*obj2)["content"] = t8ntool_ByzantiumToConstantinopleFixAt5_config;
    map_configs.addArrayObject(obj2);
}
}  // namespace retesteth::options
