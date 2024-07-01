#include <retesteth/configs/Options.h>
using namespace std;
using namespace dataobject;

namespace retesteth::options
{
genArrowGlacierToParisAtDiffC0000Cfg::genArrowGlacierToParisAtDiffC0000Cfg()
{

const string t8ntool_ArrowGlacierToParisAtDiffC0000_config = R"({
    "params" : {
        "fork" : "ArrowGlacierToMergeAtDiffC0000",
        "constantinopleForkBlock" : "0x00",
        "byzantiumForkBlock" : "0x00",
        "homesteadForkBlock" : "0x00",
        "terminalTotalDifficulty" : "0x0C0000",
        "chainID" : "0x01"
    },
    "accounts" : {
    }
})";


    spDataObject obj2(new DataObject());
    (*obj2)["path"] = "default/genesis/ArrowGlacierToParisAtDiffC0000.json";
    (*obj2)["content"] = t8ntool_ArrowGlacierToParisAtDiffC0000_config;
    map_configs.addArrayObject(obj2);
}
}  // namespace retesteth::options
