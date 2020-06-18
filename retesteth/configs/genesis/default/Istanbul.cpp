#include <retesteth/configs/Genesis.h>
#include <string>
using namespace std;

const string default_Istanbul_config = R"({
    "params" : {
        "homesteadForkBlock" : "0x00",
        "EIP150ForkBlock" : "0x00",
        "EIP158ForkBlock" : "0x00",
        "byzantiumForkBlock" : "0x00",
        "constantinopleForkBlock" : "0x00",
        "constantinopleFixForkBlock" : "0x00",
        "istanbulForkBlock" : "0x00",
        "chainID" : "0x01"
    },
    "accounts" : {
    }
})";

const string t8ntool_Istanbul_config = R"({
    "params" : {
        "fork" : "Istanbul",
        "constantinopleForkBlock" : "0x00",
        "byzantiumForkBlock" : "0x00",
        "homesteadForkBlock" : "0x00"
    },
    "accounts" : {
    }
})";
