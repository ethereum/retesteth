#include <retesteth/configs/Options.h>
#include <string>
using namespace std;
using namespace dataobject;

namespace retesteth::options
{
const string t8ntooleip_London1884_config = R"({
    "params" : {
        "fork" : "London+1884",
        "constantinopleForkBlock" : "0x00",
        "byzantiumForkBlock" : "0x00",
        "homesteadForkBlock" : "0x00"
    },
    "accounts" : {
    }
})";

t8ntooleip_genLondon1884Cfg::t8ntooleip_genLondon1884Cfg()
{
    spDataObject obj2;
    (*obj2)["path"] = "t8ntooleip/genesis/London+1884.json";
    (*obj2)["content"] = t8ntooleip_London1884_config;
    map_configs.addArrayObject(obj2);
}
}  // namespace retesteth::options
