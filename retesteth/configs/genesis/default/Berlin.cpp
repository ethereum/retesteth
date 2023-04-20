#include <retesteth/configs/Options.h>
using namespace std;
using namespace dataobject;

namespace retesteth::options
{
genBerlinCfg::genBerlinCfg()
{

const string t8ntool_Berlin_config = R"({
    "params" : {
        "fork" : "Berlin",
        "constantinopleForkBlock" : "0x00",
        "byzantiumForkBlock" : "0x00",
        "homesteadForkBlock" : "0x00",
        "chainID" : "0x01"
    },
    "accounts" : {
    }
})";


    spDataObject obj2;
    (*obj2)["path"] = "default/genesis/Berlin.json";
    (*obj2)["content"] = t8ntool_Berlin_config;
    map_configs.addArrayObject(obj2);
}
}  // namespace retesteth::options
