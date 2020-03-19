# retesteth
testeth via RPC (wiki: https://github.com/ethereum/retesteth/wiki)  
(Execution stats: http://retesteth.ethdevops.io/)

A test generaion tool for the test fillers https://github.com/ethereum/tests/tree/develop/src  
Building instruction for beginners: [retesteth + solidity build](https://github.com/ethereum/retesteth#building-instructions-for-beginners)

# The Goal

* A test tool that would be capable of running current Blockchain tests against any client by sending rpc request to the client instatnce on either local or remote host. (using unix or TCP sockets) 
* Filling existing tests (generating post state from *Filler.json instruction files) using rpc and any exisiting client
* Running rpc request - response tests with a provided client on localhost
* Bunch tests execution with many clients with many threads
* A minimum set of additional rpc methods for client to negotiate with the tool: https://github.com/ethereum/retesteth/wiki/RPC-Methods

# Current progress

* done: State tests execution and filling was done as PoC on ethereum cpp client (aleth)
* done: Tests execution using threads on localhost with multimple instances of a client (geth + aleth)
* done: Develop minimum set of rpc methods that are to be implemented on other clients and could be used to run tests via rpc
* done: PoC Running Blockchain tests using geth client
* done: Implement a set of PoC methods in other client then aleth
* done: Refactoring and stability when generating GeneralStateTests
* done: Blockchain test generation support
* now: Use retesteth to produce fork tests with geth/besu/aleth
* now: Refactor the code, impove stability

# Building instructions
```
git clone git@github.com:ethereum/retesteth.git
cd retesteth
mkdir build
cd build
cmake ..
make -j4
```
Or try building instruction for begginers: [retesteth + solidity build](https://github.com/ethereum/retesteth#building-instructions-for-beginners)


# Usage
WIKI: https://github.com/ethereum/retesteth/wiki  
Requres to have a client installed on your system. Read the wiki page on detailed instruction on how to configure your client to work with `retesteth`
https://github.com/ethereum/retesteth/wiki/Add-client-configuration-to-Retesteth
```
./retesteth -t GeneralStateTests
```

# Docker instructions
See: https://github.com/ethereum/retesteth/wiki/Docker-instructions

# RPC Methods
See: https://github.com/ethereum/retesteth/wiki/RPC-Methods

# Contact
Telegram: @wdimitry

# Building instructions for beginners
## Dependencies

#### GIT
```
sudo apt-get update
sudo apt-get install git g++ build-essential
```
#### Cmake
Make sure the version is higher than VERSION 3.9.3, otherwise install cmake from a different source
```
sudo apt-cache policy cmake
sudo apt-get install cmake
```

#### Retesteth
```
cd ~
mkdir Ethereum
cd Ethereum
git clone https://github.com/ethereum/retesteth.git
cd retesteth
mkdir build
cd build
cmake ..
```

Now you should see the successful build files generation result: 
```
Configuring done
-- Generating done
-- Build files have been written 
```

Run the build command to compile:
-j threadCount to optimize the build speed. Depending on your processor threads it will increase the building speed.
```
make -j4
```

#### Solidity

check the available boost version by  
`sudo apt-cache policy libboost-all-dev`

install boost dependency boost if version >=1.65  
`sudo apt-get install libboost-all-dev`

Solidity building instructions:

```
cd ~
cd Ethereum
git clone https://github.com/ethereum/solidity.git
cd solidity
git checkout 8f2595957bfc0f3cd18ca29240dabcd6b2122dfd
mkdir build
cd build
cmake .. -DLLL=1
make lllc -j4
```

### DONE!


