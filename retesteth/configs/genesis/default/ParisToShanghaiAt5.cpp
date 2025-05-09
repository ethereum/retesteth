#include <retesteth/configs/Options.h>
using namespace std;
using namespace dataobject;

namespace retesteth::options
{
genParisToShanghaiAtTime15kCfg::genParisToShanghaiAtTime15kCfg()
{

const string t8ntool_ParisToShanghaiAtTime15k_config = R"({
    "params" : {
        "fork" : "MergeToShanghaiAtTime15k",
        "constantinopleForkBlock" : "0x00",
        "byzantiumForkBlock" : "0x00",
        "homesteadForkBlock" : "0x00",
        "terminalTotalDifficulty" : "0x00",
        "chainID" : "0x01"
    },
    "accounts" : {
    }
})";

    spDataObject obj2(new DataObject());
    (*obj2)["path"] = "default/genesis/ParisToShanghaiAtTime15k.json";
    (*obj2)["content"] = t8ntool_ParisToShanghaiAtTime15k_config;
    map_configs.addArrayObject(obj2);
}
}  // namespace retesteth::options
