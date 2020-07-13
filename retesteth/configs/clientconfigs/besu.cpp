#include <retesteth/configs/Options.h>
using namespace std;
using namespace dataobject;

string const besu_config = R"({
    "name" : "Hyperledger Besu on TCP",
    "socketType" : "tcp",
    "socketAddress" : [
        "127.0.0.1:47710",
        "127.0.0.1:47711",
        "127.0.0.1:47712",
        "127.0.0.1:47713"
        "127.0.0.1:47714",
        "127.0.0.1:47715",
        "127.0.0.1:47716",
        "127.0.0.1:47717"
    ],
    "forks" : [
        "Frontier",
        "Homestead",
        "EIP150",
        "EIP158",
        "Byzantium",
        "Constantinople",
        "ConstantinopleFix",
        "Istanbul",
        "Berlin"
    ],
    "additionalForks" : [
        "FrontierToHomesteadAt5",
        "HomesteadToEIP150At5",
        "EIP158ToByzantiumAt5",
        "HomesteadToDaoAt5",
        "ByzantiumToConstantinopleFixAt5"
    ],
    "exceptions" : {
        "ExtraDataTooBig" : "extra-data too long",
        "InvalidDifficulty" : "invalid difficulty"
    }
})";

besucfg::besucfg()
{
    DataObject obj;
    obj["path"] = "besu/config";
    obj["content"] = besu_config;
    map_configs.addArrayObject(obj);
}
