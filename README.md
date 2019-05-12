# retesteth
testeth via RPC

A test generaion tool for the test fillers https://github.com/ethereum/tests/tree/develop/src

**! notice: This is a prototype and WIP !**

# The Goal

* A test tool that would be capable of running current Blockchain tests against any client by sending rpc request to the client instatnce on either local or remote host. (using unix or TCP sockets) 
* Filling existing tests (generating post state from *Filler.json instruction files) using rpc and any exisiting client
* Running rpc request - response tests with a provided client on localhost
* Bunch tests execution with many clients with many threads
* A minimum set of additional rpc methods for client to negotiate with the tool: https://github.com/ethereum/retesteth/issues/5

# Current progress

* done: State tests execution and filling was done as PoC on ethereum-cpp client
* done: Tests execution using threads on localhost with multimple instances of a client
* done: Develop minimum set of rpc methods that are to be implemented on other clients and could be used to run tests via rpc
* done: PoC Running Blockchain tests using aleth client
* now: Implement a set of PoC methods in other client then aleth

# Building instructions
```
git clone git@github.com:ethereum/retesteth.git
cd retesteth
mkdir build
cd build
cmake ..
make -j4
```

# Usage
Requres to have a client installed on your system. Read the wiki page on detailed instruction on how to configure your client to work with `retesteth`
https://github.com/ethereum/retesteth/wiki/Add-client-configuration-to-Retesteth
```
./retesteth -t BCGeneralStateTests
```

# Docker instructions
See: https://github.com/ethereum/retesteth/wiki/Docker-instructions

# RPC Methods
See: https://github.com/ethereum/retesteth/wiki/RPC-Methods

# Contact
Skype: wins_vega
