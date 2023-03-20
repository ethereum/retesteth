#!/bin/bash
retesteth -t BlockchainTests/ValidBlocks/bcExample -- --singletest optionsTest --filltests &>/dev/null

function findsubstr()
{
    local STR=$1
    local SUB=$2
    if [[ "$STR" == *"$SUB"* ]]; then
        echo 1
        return
    fi
    echo 0
}

function countsubstr()
{
    local STR=$1
    local SUB=$2
    local s=${STR//"$SUB"}
    echo "$(((${#STR} - ${#s}) / ${#SUB}))"
}

# Test --poststate without block selection
echo "Testing --poststate 2:10 in Fill mode"
out=$(retesteth -t BlockchainTests/ValidBlocks/bcExample -- --singletest optionsTest --poststate 2:10 --filltests --singlenet London 2>/dev/null)
out=$(echo $out)

if (( $(countsubstr "$out" "State Dump:") != 0 ));then
    echo "ERROR: Expected no State Dump!"
fi

# Test --poststate without block selection
echo "Testing --poststate 3:0 in Fill mode"
out=$(retesteth -t BlockchainTests/ValidBlocks/bcExample -- --singletest optionsTest --poststate 3:0 --filltests --singlenet London 2>/dev/null)
out=$(echo $out)

if (( $(countsubstr "$out" "State Dump:") != 0 ));then
    echo "ERROR: Expected no State Dump!"
fi

# Test --poststate without block selection
echo "Testing --poststate 2:0 in Fill mode"
out=$(retesteth -t BlockchainTests/ValidBlocks/bcExample -- --singletest optionsTest --poststate 2:0 --filltests --singlenet London 2>/dev/null)
out=$(echo $out)

if (( $(findsubstr "$out" "storage\" : { \"0x01\" : \"0x01\", \"0x02\" : \"0x01\" }") != 1 ));then
    echo "ERROR: Final post not found!"
fi
if (( $(findsubstr "$out" "nonce\" : \"0x02\"") != 1 ));then
    echo "ERROR: Nonce expected to have sent 2 txs"
fi
if (( $(countsubstr "$out" "State Dump:") != 1 ));then
    echo "ERROR: Expected only one State Dump!"
fi

# Test --poststate without block selection
echo "Testing --poststate in Fill mode"
out=$(retesteth -t BlockchainTests/ValidBlocks/bcExample -- --singletest optionsTest --poststate --filltests --singlenet London 2>/dev/null)
out=$(echo $out)

if (( $(findsubstr "$out" "storage\" : { \"0x01\" : \"0x01\", \"0x02\" : \"0x01\", \"0x03\" : \"0x01\", \"0x05\" : \"0x01\" }") != 1 ));then
    echo "ERROR: Final post not found!"
fi
if (( $(findsubstr "$out" "nonce\" : \"0x04\"") != 1 ));then
    echo "ERROR: Nonce expected to have sent 4 txs"
fi
if (( $(countsubstr "$out" "State Dump:") != 1 ));then
    echo "ERROR: Expected only one State Dump!"
fi

# Test --poststate without block selection
echo "Testing --poststate in Run mode"
out=$(retesteth -t BlockchainTests/ValidBlocks/bcExample -- --singletest optionsTest --poststate --singlenet London 2>/dev/null)
out=$(echo $out)

if (( $(findsubstr "$out" "storage\" : { \"0x01\" : \"0x01\", \"0x02\" : \"0x01\", \"0x03\" : \"0x01\", \"0x05\" : \"0x01\" }") != 1 ));then
    echo "ERROR: Final post not found!"
fi
if (( $(findsubstr "$out" "nonce\" : \"0x04\"") != 1 ));then
    echo "ERROR: Nonce expected to have sent 4 txs"
fi
if (( $(countsubstr "$out" "State Dump:") != 1 ));then
    echo "ERROR: Expected only one State Dump!"
fi

# Test --poststate with block selection
echo "Testing --poststate x:y in Run mode"
out=$(retesteth -t BlockchainTests/ValidBlocks/bcExample -- --singletest optionsTest --poststate 2:0 --singlenet London 2>/dev/null)
out=$(echo $out)

if (( $(findsubstr "$out" "storage\" : { \"0x01\" : \"0x01\", \"0x02\" : \"0x01\", \"0x03\" : \"0x01\" }") != 1 ));then
    echo "ERROR: Final post not found!"
fi
if (( $(findsubstr "$out" "nonce\" : \"0x03\"") != 1 ));then
    echo "ERROR: Nonce expected to have sent 3 txs"
fi
if (( $(countsubstr "$out" "State Dump:") != 1 ));then
    echo "ERROR: Expected only one State Dump!"
fi

