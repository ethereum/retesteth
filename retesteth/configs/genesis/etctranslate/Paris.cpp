#include <retesteth/configs/Options.h>
using namespace std;
using namespace dataobject;

namespace retesteth::options
{
genParisCfgETCTR::genParisCfgETCTR()
{
const string t8ntool_Paris_config = R"({
    "params" : {
        "fork" : "ETC_Mystique",
        "terminalTotalDifficulty" : "0x00",
        "constantinopleForkBlock" : "0x00",
        "byzantiumForkBlock" : "0x00",
        "homesteadForkBlock" : "0x00",
        "chainID" : "0x3d"
    },
    "accounts" : {
    }
})";

    {
        spDataObject obj;
        (*obj)["path"] = "etctranslate/genesis/Paris.json";
        (*obj)["content"] = t8ntool_Paris_config;
        map_configs.addArrayObject(obj);
    }
}
}  // namespace retesteth::options
