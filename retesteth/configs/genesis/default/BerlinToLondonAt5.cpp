#include <retesteth/configs/Options.h>
#include <string>
using namespace std;
using namespace dataobject;

namespace retesteth::options
{
const string default_BerlinToLondonAt5_config = R"({
    "params" : {
      "homesteadForkBlock" : "0x00",
      "EIP150ForkBlock" : "0x00",
      "EIP158ForkBlock" : "0x00",
      "byzantiumForkBlock" : "0x00",
      "constantinopleForkBlock" : "0x00",
      "constantinopleFixForkBlock" : "0x00",
      "berlinForkBlock" : "0x00",
      "londonForkBlock" : "0x05"
    },
    "accounts" : {
    }
})";

const string t8ntool_BerlinToLondonAt5_config = R"({
    "params" : {
        "fork" : "BerlinToLondonAt5",
        "constantinopleForkBlock" : "0x00",
        "byzantiumForkBlock" : "0x00",
        "homesteadForkBlock" : "0x00",
        "londonForkBlock" : "0x05"
    },
    "accounts" : {
    }
})";

genBerlinToLondonCfg::genBerlinToLondonCfg()
{
    {
        spDataObject obj;
        (*obj)["path"] = "besu/genesis/BerlinToLondonAt5.json";
        (*obj)["content"] = default_BerlinToLondonAt5_config;
        map_configs.addArrayObject(obj);
    }
    {
        spDataObject obj;
        (*obj)["path"] = "default/genesis/BerlinToLondonAt5.json";
        (*obj)["content"] = t8ntool_BerlinToLondonAt5_config;
        map_configs.addArrayObject(obj);
    }
}
}  // namespace retesteth::options
