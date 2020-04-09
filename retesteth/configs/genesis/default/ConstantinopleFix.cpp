#include <retesteth/configs/Genesis.h>
#include <string>
using namespace std;

string default_ConstantinopleFix_config = R"({
    "params" : {
       "homesteadForkBlock" : "0x00",
       "EIP150ForkBlock" : "0x00",
       "EIP158ForkBlock" : "0x00",
       "byzantiumForkBlock" : "0x00",
       "constantinopleForkBlock" : "0x00",
       "constantinopleFixForkBlock" : "0x00"
    }
})";
