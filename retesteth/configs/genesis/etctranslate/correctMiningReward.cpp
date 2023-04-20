#include <retesteth/configs/Options.h>
using namespace std;
using namespace dataobject;

namespace retesteth::options
{
genRewardsCfgETCTR::genRewardsCfgETCTR()
{
const string t8ntool_correctMiningReward_config = R"({
    "//comment" : "State Tests does not calculate mining reward in post conditions, so when filling a blockchain test out of it, the mining reward must be set",
    "Frontier":          "5000000000000000000",
    "Homestead":         "5000000000000000000",
    "EIP150":            "5000000000000000000",
    "EIP158":            "5000000000000000000",

    "Byzantium":         "4000000000000000000",
    "Constantinople":    "4000000000000000000",
    "ConstantinopleFix": "4000000000000000000",
    "Istanbul":          "3200000000000000000",
    "Berlin" :           "3200000000000000000",
    "London" :           "3200000000000000000",
    "Merge" :            "3200000000000000000",

    "ETC_Atlantis" :     "4000000000000000000",
    "ETC_Agharta" :      "4000000000000000000",
    "ETC_Phoenix" :      "3200000000000000000",
    "ETC_Magneto" :      "3200000000000000000",
    "ETC_Mystique" :     "3200000000000000000"
})";

    spDataObject obj2;
    (*obj2)["path"] = "etctranslate/genesis/correctMiningReward.json";
    (*obj2)["content"] = t8ntool_correctMiningReward_config;
    map_configs.addArrayObject(obj2);
}
}  // namespace retesteth::options
