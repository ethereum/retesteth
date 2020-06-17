#include <retesteth/configs/Genesis.h>
#include <string>
using namespace std;

const string default_EIP158_config = R"({
    "params" : {
       "homesteadForkBlock" : "0x00",
       "EIP150ForkBlock" : "0x00",
       "EIP158ForkBlock" : "0x00"
    },
    "accounts" : {
    }
})";

const string t8ntool_EIP158_config = R"({
    "params" : {
        "fork" : "EIP158",
        "homesteadForkBlock" : "0x00"
    },
    "accounts" : {
    }
})";
