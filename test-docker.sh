#!/bin/bash

GETH_IMAGE="${GETH_IMAGE:-ethereum/client-go:v1.9.10}"
RETESTETH_IMAGE="${RETESTETH_IMAGE:-retesteth:local}"


buildImage () {
    docker build -t "$RETESTETH_IMAGE" .
}

stopGeth() {
    for i in {0..7}; do
        echo -n "stoping $NAME:"
        NAME="geth$i"
        docker rm -f "$NAME"
    done
}

startGeth () {
    # Geth
    for i in {0..7}; do
        NAME="geth$i"
        echo -n "starting $NAME:"
        docker run -d --net=host --name "$NAME" "$GETH_IMAGE" retesteth --rpcport $((8545 + i))
    done
}

startRetesteth() {
    startGeth
    docker run -i --rm --net=host --name "retesteth" "$RETESTETH_IMAGE" "$@"
    stopGeth
}


stopRetesteth () {
    docker rm -f "retesteth"
}


case $1 in
    "start")
        startRetesteth -t GeneralStateTests -- --all -j10 --testpath /tests
        startRetesteth -t LegacyTests -- --all -j10 --testpath /tests
        startRetesteth -t BlockchainTests -- --all -j10 --testpath /tests
        ;;
    "stop")
        stopRetesteth
        stopGeth
        ;;
    "build")
        buildImage
        ;;
esac
