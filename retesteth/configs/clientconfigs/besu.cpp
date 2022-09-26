#include <retesteth/configs/Options.h>
using namespace std;
using namespace dataobject;

namespace retesteth::options
{
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
    "initializeTime" : "5",
    "forks" : [
        "Frontier",
        "Homestead",
        "EIP150",
        "EIP158",
        "Byzantium",
        "Constantinople",
        "ConstantinopleFix",
        "Istanbul",
        "Berlin",
        "London"
    ],
    "additionalForks" : [
        "FrontierToHomesteadAt5",
        "HomesteadToEIP150At5",
        "EIP158ToByzantiumAt5",
        "HomesteadToDaoAt5",
        "ByzantiumToConstantinopleFixAt5",
        "BerlinToLondonAt5",
        "ArrowGlacier",
        "GrayGlacier"
    ],
    "exceptions" : {
        "ExtraDataTooBig" : "extra-data too long",
        "InvalidDifficulty" : "invalid difficulty"
    }
})";

string const besu_start = R"(#!/bin/sh
threads=1
if [ "${1:-0}" -gt 1 ]
then
    threads=$1
fi

mkdir ~/.retesteth/logs
i=0
while [ "$i" -lt $threads ]; do
    tmpdir=$(mktemp -d -t ci-XXXXXXXXXX)
    file=$(date +"%m-%d-%y-%s")
    besu retesteth --rpc-http-port $((47710+$i)) --data-path=$tmpdir --logging=DEBUG >> ~/.retesteth/logs/besu-$file &
    i=$(( i + 1 ))
done
)";

string const besu_stop = R"(#!/bin/sh
killall -9 java
)";

besucfg::besucfg()
{
    {
        spDataObject obj;
        (*obj)["path"] = "besu/config";
        (*obj)["content"] = besu_config;
        map_configs.addArrayObject(obj);
    }
    {
        spDataObject obj;
        (*obj)["exec"] = true;
        (*obj)["path"] = "besu/start.sh";
        (*obj)["content"] = besu_start;
        map_configs.addArrayObject(obj);
    }
    {
        spDataObject obj;
        (*obj)["exec"] = true;
        (*obj)["path"] = "besu/stop.sh";
        (*obj)["content"] = besu_stop;
        map_configs.addArrayObject(obj);
    }
}
}  // namespace retesteth::options
