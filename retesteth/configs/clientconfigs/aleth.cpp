#include <retesteth/configs/Options.h>
using namespace std;
using namespace dataobject;

namespace retesteth::options
{
string const aleth_config = R"({
    "name" : "Ethereum cpp-client",
    "socketType" : "ipc",
    "socketAddress" : "aleth.sh",
    "initializeTime" : "5",
    "forks" : [
        "Frontier",
        "Homestead",
        "EIP150",
        "EIP158",
        "Byzantium",
        "Constantinople",
        "ConstantinopleFix",
        "Istanbul"
    ],
    "additionalForks" : [
        "FrontierToHomesteadAt5",
        "HomesteadToEIP150At5",
        "EIP158ToByzantiumAt5",
        "HomesteadToDaoAt5",
        "ByzantiumToConstantinopleFixAt5"
    ],
    "exceptions" : {
        "ExtraDataTooBig" : "ExtraData too big",
        "InvalidDifficulty" : "Invalid Difficulty",
        "InvalidGasLimit" : "Invalid Block GasLimit",
        "TooMuchGasUsed" : "Too much gas used",
        "InvalidNumber" : "Invalid number",
        "InvalidTimestamp" : "Invalid timestamp",
        "InvalidLogBloom" : "Invalid log bloom",
        "InvalidStateRoot" : "Invalid state root",
        "InvalidGasLimit2" : "Invalid Block GasLimit",
        "InvalidGasUsed" : "Invalid gas used",
        "InvalidBlockMixHash" : "Invalid block nonce",
        "InvalidBlockNonce" : "Invalid block nonce"
        "UnknownParent" : "UnknownParent",
        "UnknownParent2" : "Invalid parent hash",
        "InvalidReceiptsStateRoot" : "Invalid receipts state root",
        "InvalidTransactionsRoot" : "Invalid transactions root",
        "InvalidUnclesHash" : "Invalid uncles hash"
    }
})";

string const aleth_config_sh = R"(#!/bin/bash
##onexit()
##{
##    kill $child
##}
##trap onexit SIGTERM
##trap onexit SIGABRT

##ARGUMENTS PASSED BY RETESTETH
##
## $1  <db-path> 		A path to database directory that a client should use
## $2  <ipcpath>		A path to the ipc socket file (has /geth.ipc at the end)
#####

aleth --test --db-path $1 --ipcpath $2 --log-verbosity 5 &
##child=$!
##wait "$child"
)";

string const aleth_stop_sh = R"(#!/bin/bash
killall aleth
)";


alethcfg::alethcfg()
{
    {
        spDataObject obj;
        (*obj)["path"] = "aleth/config";
        (*obj)["content"] = aleth_config;
        map_configs.addArrayObject(obj);
    }
    {
        spDataObject obj;
        (*obj)["path"] = "aleth/aleth.sh";
        (*obj)["content"] = aleth_config_sh;
        map_configs.addArrayObject(obj);
    }
    /*{
        DataObject obj;
        obj["path"] = "aleth/start.sh";
        obj["content"] = aleth_config_sh;
        map_configs.addArrayObject(obj);
    }*/
    {
        spDataObject obj;
        (*obj)["exec"] = true;
        (*obj)["path"] = "aleth/stop.sh";
        (*obj)["content"] = aleth_stop_sh;
        map_configs.addArrayObject(obj);
    }
}
}  // namespace retesteth::options
