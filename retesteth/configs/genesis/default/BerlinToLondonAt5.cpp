#include <retesteth/configs/Options.h>
using namespace std;
using namespace dataobject;

namespace retesteth::options
{
genBerlinToLondonCfg::genBerlinToLondonCfg()
{

const string t8ntool_BerlinToLondonAt5_config = R"({
    "params" : {
        "fork" : "BerlinToLondonAt5",
        "constantinopleForkBlock" : "0x00",
        "byzantiumForkBlock" : "0x00",
        "homesteadForkBlock" : "0x00",
        "londonForkBlock" : "0x05",
        "chainID" : "0x01"
    },
    "accounts" : {
    }
})";

    {
        spDataObject obj;
        (*obj)["path"] = "default/genesis/BerlinToLondonAt5.json";
        (*obj)["content"] = t8ntool_BerlinToLondonAt5_config;
        map_configs.addArrayObject(obj);
    }
}
}  // namespace retesteth::options
