#include <retesteth/configs/Genesis.h>
#include <string>
using namespace std;

const string default_EIP158ToByzantiumAt5_config = R"({
    "params" : {
        "homesteadForkBlock" : "0x00",
        "EIP150ForkBlock" : "0x00",
        "EIP158ForkBlock" : "0x00",
        "byzantiumForkBlock" : "0x05"
    },
    "accounts" : {
    }
})";

const string t8ntool_EIP158ToByzantiumAt5_config = R"({
    "params" : {
        "fork" : "EIP158ToByzantiumAt5",
        "byzantiumForkBlock" : "0x05",
        "homesteadForkBlock" : "0x00"
    },
    "accounts" : {
    }
})";
