# retesteth
testeth via RPC

A test generaion tool for the test fillers https://github.com/ethereum/tests/tree/develop/src

**! notice: This is a prototype and WIP. The tool is not ready for use !**

# The Goal

* A testtool that would be capable of running current StateTests + Blockchain tests against any client by sending rpc request to the client instatnce on either local or remote host. 
* Filling existing tests (generating post state from *Filler.json instruction files) using rpc and any exisiting client
* Running rpc request - response tests with a provided client on localhost
* Bunch tests execution with many clients with many threads
* A minimum set of additional rpc methods for client to negotiate with the tool: https://github.com/ethereum/retesteth/issues/5

# Current progress

* done: State tests execution and filling was done as PoC on ethereum-cpp client
* done: Tests execution using threads on localhost with multimple instances of a client
* now:  Develop cpp-ethereum rpc methods that are implemented on other clients and could be used to run tests via rpc

# Building instructions
```
git clone git@github.com:ethereum/retesteth.git
cd retesteth
mkdir build
cd build
cmake ..
make -j4
```
