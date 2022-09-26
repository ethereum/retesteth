#include <retesteth/configs/Options.h>
using namespace std;
using namespace dataobject;

namespace retesteth::options
{
string const alethIPCDebug_config = R"({
    "name" : "Ethereum aleth on IPC Debug",
    "socketType" : "ipc-debug",
    "socketAddress" : "/home/username/.ethereum/geth.ipc",
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

alethIpcDebugcfg::alethIpcDebugcfg()
{
    spDataObject obj;
    (*obj)["path"] = "alethIPCDebug/config";
    (*obj)["content"] = alethIPCDebug_config;
    map_configs.addArrayObject(obj);
}
}  // namespace retesteth::options
