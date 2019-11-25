#include <retesteth/configs/Genesis.h>
#include <string>
using namespace std;

string default_HomesteadToDaoAt5_config = R"({
    "params" : {
        "homesteadForkBlock" : "0x00",
        "daoForkBlock" : "0x05"
    },
    "accounts" : {
        "0x0000000000000000000000000000000000000001" : {
            "precompiled" : {
                "name" : "ecrecover",
                "linear" : {
                    "base" : 3000,
                    "word" : 0
                }
            }
        },
        "0x0000000000000000000000000000000000000002" : {
            "precompiled" : {
                "name" : "sha256",
                "linear" : {
                    "base" : 60,
                    "word" : 12
                }
            }
        },
        "0x0000000000000000000000000000000000000003" : {
            "precompiled" : {
                "name" : "ripemd160",
                "linear" : {
                    "base" : 600,
                    "word" : 120
                }
            }
        },
        "0x0000000000000000000000000000000000000004" : {
            "precompiled" : {
                "name" : "identity",
                "linear" : {
                    "base" : 15,
                    "word" : 3
                }
            }
        }
    }
})";
