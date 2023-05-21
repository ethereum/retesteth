#include <retesteth/configs/Options.h>
using namespace std;
using namespace dataobject;

namespace retesteth::options
{
genConstantinopleFixCfgETCTR::genConstantinopleFixCfgETCTR()
{

const string t8ntool_ConstantinopleFix_config = R"({
    "params" : {
        "fork" : "ETC_Agharta",
        "constantinopleForkBlock" : "0x00",
        "byzantiumForkBlock" : "0x00",
        "homesteadForkBlock" : "0x00",
        "chainID" : "0x3d"
    },
    "accounts" : {
    }
})";

    spDataObject obj2;
    (*obj2)["path"] = "etctranslate/genesis/ConstantinopleFix.json";
    (*obj2)["content"] = t8ntool_ConstantinopleFix_config;
    map_configs.addArrayObject(obj2);
}
}  // namespace retesteth::options
