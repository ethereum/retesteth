#include <retesteth/configs/Genesis.h>
#include <string>
using namespace std;

const string default_Homestead_config = R"({
    "params" : {
        "homesteadForkBlock" : "0x00"
    },
    "accounts" : {
    }
})";

const string t8ntool_Homestead_config = R"({
    "params" : {
        "fork" : "Homestead"
    },
    "accounts" : {
    }
})";
