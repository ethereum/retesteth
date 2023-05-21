#include <retesteth/configs/Options.h>
using namespace std;
using namespace dataobject;

namespace retesteth::options
{
genHomesteadToDaoCfg::genHomesteadToDaoCfg()
{

const string t8ntool_HomesteadToDaoAt5_config = R"({
    "params" : {
        "fork" : "HomesteadToDaoAt5",
        "homesteadForkBlock" : "0x00",
        "chainID" : "0x01"
    },
    "accounts" : {
    }
})";

    spDataObject obj2;
    (*obj2)["path"] = "default/genesis/HomesteadToDaoAt5.json";
    (*obj2)["content"] = t8ntool_HomesteadToDaoAt5_config;
    map_configs.addArrayObject(obj2);
}
}  // namespace retesteth::options
