#include <retesteth/configs/Options.h>
#include <string>
using namespace std;
using namespace dataobject;

namespace retesteth::options
{
const string default_Merge_config = R"({
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
       "terminalTotalDifficulty" : "0x00",
       "chainID" : "0x01"
    },
    "accounts" : {
    }
})";

const string t8ntool_Merge_config = R"({
    "params" : {
        "fork" : "Merged",
        "terminalTotalDifficulty" : "0x00",
        "constantinopleForkBlock" : "0x00",
        "byzantiumForkBlock" : "0x00",
        "homesteadForkBlock" : "0x00"
    },
    "accounts" : {
    }
})";

genMergeCfg::genMergeCfg()
{
    {
        spDataObject obj;
        (*obj)["path"] = "besu/genesis/Merge.json";
        (*obj)["content"] = default_Merge_config;
        map_configs.addArrayObject(obj);
    }

    {
        spDataObject obj;
        (*obj)["path"] = "default/genesis/Merge.json";
        (*obj)["content"] = t8ntool_Merge_config;
        map_configs.addArrayObject(obj);
    }
}
}  // namespace retesteth::options
