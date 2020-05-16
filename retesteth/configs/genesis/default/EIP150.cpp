#include <retesteth/configs/Genesis.h>
#include <string>
using namespace std;

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
        "fork" : "EIP150"
    },
    "accounts" : {
    }
})";
