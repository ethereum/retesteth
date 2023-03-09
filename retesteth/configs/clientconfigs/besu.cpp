#include <retesteth/configs/Options.h>
using namespace std;
using namespace dataobject;

namespace retesteth::options
{
string const besu_config = R"({
    "name" : "Hyperledger Besu on TCP",
    "socketType" : "tranition-tool",
    "socketAddress" : "start.sh",
    "initializeTime" : "5",
    "checkDifficulty" : true,
    "calculateDifficulty" : false,
    "checkBasefee" : true,
    "calculateBasefee" : false,
    "checkLogsHash" : true,
    "support1559" : true,
    "transactionsAsJson" : false,
    "tmpDir" : "/dev/shm",
    "defaultChainID" : 1,
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
        "London",
        "Merge",
        "Shanghai"
    ],
    "additionalForks" : [
        "FrontierToHomesteadAt5",
        "HomesteadToEIP150At5",
        "EIP158ToByzantiumAt5",
        "HomesteadToDaoAt5",
        "ByzantiumToConstantinopleFixAt5",
        "BerlinToLondonAt5",
        "ArrowGlacier",
        "GrayGlacier",
        "MergeToShanghaiAtTime15k"
    ],
    "fillerSkipForks" : [
        "Merge+3540+3670",
        "Merge+3860",
        "Merge+3855"
    ],
    "exceptions" : {
        "ExtraDataTooBig" : "extra-data too long",
        "InvalidDifficulty" : "invalid difficulty"
    }
})";

string const besu_start = R"(#!/bin/sh
if [ $1 = "t8n" ] || [ $1 = "b11r" ]; then
    besuevm $1 $2 $3 $4 $5 $6 $7 $8 $9 $10 $11 $12 $13 $14 $15 $16 $17 $18 $19 $20 $21 $22 $23 $24 $25 $26
elif [ $1 = "-v" ]; then
    besuevm --version
else
    stateProvided=0
    readErrorLog=0
    errorLogFile=""
    cmdArgs=""
    for index in ${1} ${2} ${3} ${4} ${5} ${6} ${7} ${8} ${9} ${10} ${11} ${12} ${13} ${14} ${15} ${16} ${17} ${18} ${19} ${20} ${21} ${22} ${23} ${24} ${25} ${26}; do
        if [ $index = "--input.alloc" ]; then
            stateProvided=1
        fi
        if [ $readErrorLog -eq 1 ]; then
            errorLogFile=$index
            readErrorLog=0
            continue
        fi
        if [ $index = "--output.errorlog" ]; then
            readErrorLog=1
            continue
        fi
        cmdArgs=$cmdArgs" "$index
    done
    if [ $stateProvided -eq 1 ]; then
        besuevm t8n $cmdArgs 2> $errorLogFile
    else
        besuevm t9n $cmdArgs 2> $errorLogFile
    fi
fi
)";

string const besu_stop = R"(#!/bin/sh
killall -9 java
)";

genbesucfg::genbesucfg()
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
    {
        spDataObject obj;
        (*obj)["exec"] = true;
        (*obj)["path"] = "besu/yul.sh";
        (*obj)["content"] = yul_compiler_sh;
        map_configs.addArrayObject(obj);
    }
}
}  // namespace retesteth::options
