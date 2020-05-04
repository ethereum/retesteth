#include <retesteth/configs/Genesis.h>
#include <string>
using namespace std;

const string default_Constantinople_config = R"({
    "params" : {
       "homesteadForkBlock" : "0x00",
       "EIP150ForkBlock" : "0x00",
       "EIP158ForkBlock" : "0x00",
       "byzantiumForkBlock" : "0x00",
       "constantinopleForkBlock" : "0x00"
    },
    "accounts" : {
    }
})";

const string t8ntool_Constantinople_config = R"({
    "params" : {
        "fork" : "Constantinople"
    },
    "accounts" : {
    }
})";
