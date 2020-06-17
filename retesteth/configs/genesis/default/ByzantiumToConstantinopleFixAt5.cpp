#include <retesteth/configs/Genesis.h>
#include <string>
using namespace std;

const string default_ByzantiumToConstantinopleFixAt5_config = R"({
    "params" : {
      "homesteadForkBlock" : "0x00",
      "EIP150ForkBlock" : "0x00",
      "EIP158ForkBlock" : "0x00",
      "byzantiumForkBlock" : "0x00",
      "constantinopleForkBlock" : "0x05",
      "constantinopleFixForkBlock" : "0x05"
    },
    "accounts" : {
    }
})";

const string t8ntool_ByzantiumToConstantinopleFixAt5_config = R"({
    "params" : {
        "fork" : "ByzantiumToConstantinopleFixAt5",
        "constantinopleForkBlock" : "0x05",
        "byzantiumForkBlock" : "0x00",
        "homesteadForkBlock" : "0x00"
    },
    "accounts" : {
    }
})";
