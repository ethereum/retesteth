#include <retesteth/configs/Genesis.h>
#include <string>
using namespace std;

const string default_FrontierToHomesteadAt5_config = R"({
    "params" : {
        "homesteadForkBlock" : "0x05"
    },
    "accounts" : {
    }
})";

const string t8ntool_FrontierToHomesteadAt5_config = R"({
    "params" : {
        "fork" : "FrontierToHomesteadAt5",
        "homesteadForkBlock" : "0x05"
    },
    "accounts" : {
    }
})";
