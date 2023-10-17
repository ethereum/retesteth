#include <retesteth/configs/Options.h>
using namespace std;
using namespace dataobject;

namespace retesteth::options
{
genRewardsCfgETC::genRewardsCfgETC()
{

const string etc_correctMiningReward_config = R"({
    "//comment" : "State Tests does not calculate mining reward in post conditions, so when filling a blockchain test out of it, the mining reward must be set",
    "Frontier":          "5000000000000000000",
    "Homestead":         "5000000000000000000",
    "EIP150":            "5000000000000000000",
    "EIP158":            "5000000000000000000",

    "ETC_Atlantis" :     "4000000000000000000",
    "ETC_Agharta" :      "4000000000000000000",
    "ETC_Phoenix" :      "3200000000000000000",
    "ETC_Magneto" :      "3200000000000000000",
    "ETC_Mystique" :     "3200000000000000000",
    "ETC_Mystique2" :     "3200000000000000000",
    "ETC_Mystique3" :     "3200000000000000000"

})";


    spDataObject obj;
    (*obj)["path"] = "besuetc/genesis/correctMiningReward.json";
    (*obj)["content"] = etc_correctMiningReward_config;
    map_configs.addArrayObject(obj);

    spDataObject obj2;
    (*obj2)["path"] = "etc/genesis/correctMiningReward.json";
    (*obj2)["content"] = etc_correctMiningReward_config;
    map_configs.addArrayObject(obj2);
}
}  // namespace retesteth::options
