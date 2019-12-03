#include <retesteth/configs/Genesis.h>
#include <string>
using namespace std;

string default_ByzantiumToConstantinopleFixAt5_config = R"({
    "params" : {
      "homesteadForkBlock" : "0x00",
      "EIP150ForkBlock" : "0x00",
      "EIP158ForkBlock" : "0x00",
      "byzantiumForkBlock" : "0x00",
      "constantinopleForkBlock" : "0x05",
      "constantinopleFixForkBlock" : "0x05"
    },
    "accounts" : {
    }
})";
