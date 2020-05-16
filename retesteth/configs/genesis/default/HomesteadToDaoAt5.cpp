#include <retesteth/configs/Genesis.h>
#include <string>
using namespace std;

const string default_HomesteadToDaoAt5_config = R"({
    "params" : {
        "homesteadForkBlock" : "0x00",
        "daoHardforkBlock" : "0x05"
    },
    "accounts" : {
    }
})";

const string t8ntool_HomesteadToDaoAt5_config = R"({
    "params" : {
        "fork" : "HomesteadToDaoAt5"
    },
    "accounts" : {
    }
})";
