#include <retesteth/configs/Options.h>
using namespace std;
using namespace dataobject;

namespace retesteth::options
{
const string default_MergeToShanghaiAtTime15k_config = R"({
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
       "terminalTotalDifficulty" : "0x00",
       "shanghaiForkBlock" : "0x05",
       "chainID" : "0x01"
    },
    "accounts" : {
    }
})";

const string t8ntool_MergeToShanghaiAtTime15k_config = R"({
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

genMergeToShanghaiAtTime15kCfg::genMergeToShanghaiAtTime15kCfg()
{
    spDataObject obj(new DataObject());
    (*obj)["path"] = "besu/genesis/MergeToShanghaiAtTime15k.json";
    (*obj)["content"] = default_MergeToShanghaiAtTime15k_config;
    map_configs.addArrayObject(obj);

    spDataObject obj2(new DataObject());
    (*obj2)["path"] = "default/genesis/MergeToShanghaiAtTime15k.json";
    (*obj2)["content"] = t8ntool_MergeToShanghaiAtTime15k_config;
    map_configs.addArrayObject(obj2);
}
}  // namespace retesteth::options
