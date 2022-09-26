#include <retesteth/configs/Options.h>
#include <string>
using namespace std;
using namespace dataobject;

namespace retesteth::options
{
const string default_EIP150_config = R"({
    "params" : {
       "homesteadForkBlock" : "0x00",
       "EIP150ForkBlock" : "0x00"
    },
    "accounts" : {
    }
})";

const string t8ntool_EIP150_config = R"({
    "params" : {
        "fork" : "EIP150",
        "homesteadForkBlock" : "0x00"
    },
    "accounts" : {
    }
})";

genEIP150Cfg::genEIP150Cfg()
{
    spDataObject obj;
    (*obj)["path"] = "besu/genesis/EIP150.json";
    (*obj)["content"] = default_EIP150_config;
    map_configs.addArrayObject(obj);

    spDataObject obj2;
    (*obj2)["path"] = "default/genesis/EIP150.json";
    (*obj2)["content"] = t8ntool_EIP150_config;
    map_configs.addArrayObject(obj2);
}
}  // namespace retesteth::options
