#include <retesteth/configs/Genesis.h>
#include <string>
using namespace std;

const string default_HomesteadToEIP150At5_config = R"({
    "params" : {
        "homesteadForkBlock" : "0x00",
        "EIP150ForkBlock" : "0x05"
    },
    "accounts" : {
    }
})";

const string t8ntool_HomesteadToEIP150At5_config = R"({
    "params" : {
        "fork" : "HomesteadToEIP150At5"
    },
    "accounts" : {
    }
})";
