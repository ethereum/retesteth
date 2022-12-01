# retesteth
testeth via RPC (wiki: https://github.com/ethereum/retesteth/wiki)
tests execution/generation via transition tool (https://ethereum-tests.readthedocs.io/en/latest/t8ntool-ref.html)
(Execution stats: http://retesteth.ethdevops.io/)

* A test generation tool for the test fillers https://github.com/ethereum/tests/tree/develop/src
* Building instruction for beginners: [retesteth + solidity build](https://github.com/ethereum/retesteth#building-instructions-for-beginners)
* [Usage tutorial](https://ethereum-tests.readthedocs.io/en/latest/retesteth-tutorial.html)

# The Goal

* A test tool that would be capable of running current Blockchain/State tests against any client
* On client side use test RPC or transition tool executable which exports client core logic of transaction execution on given state
* Filling existing tests (generating post state from *Filler.json instruction files) using the above and any existing client
* Running request - response tests with a provided client on localhost
* Bunch tests execution with many clients with many threads
* A minimum set of additional RPC methods for client to negotiate with the tool: https://github.com/ethereum/retesteth/wiki/RPC-Methods
* Or a simple transition tool that is also usefull for transaction debugging: https://ethereum-tests.readthedocs.io/en/latest/t8ntool-ref.html

# Current progress

* done: State tests execution and filling was done as PoC on Ethereum cpp client (aleth)
* done: Tests execution using threads on localhost with multiple instances of a client (geth + aleth)
* done: Develop minimum set of RPC methods that are to be implemented on other clients and could be used to run tests via RPC
* done: PoC Running Blockchain tests using geth client
* done: Implement a set of PoC methods in other client then aleth
* done: Refactoring and stability when generating GeneralStateTests
* done: Blockchain test generation support
* done: Use retesteth to produce fork tests with geth/besu
* done: Refactor the code, improve stability
* now: Support and development, support teams

# Building instructions
Ubuntu (retesteth):
```
git clone git@github.com:ethereum/retesteth.git
cd retesteth
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j4
```

MacOS (retesteth + tests + geth):
```
HOMEBREW_NO_AUTO_UPDATE=1 brew install -q cmake ninja git go@1.16
git clone --depth 1 https://github.com/ethereum/go-ethereum.git
git clone --depth 1 https://github.com/ethereum/tests.git
cd go-ethereum
make all
ln -s ./build/bin/evm /usr/local/bin/evm
cd ..
git clone https://github.com/ethereum/retesteth.git
cd retesteth
mkdir build
cd build
cmake .. -G Ninja -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE=../toolchain.cmake
cmake --build .

```

MacOS / Ubuntu dependecy issues:
If one of the following dependecies is failing to autoinstall, some times due to outdated version or fails to build on OS, you can install it locally from a version that works (newer) and select in `-DLOCALDEPS` flag
```
rm -r /root/.hunter
rm CMakeCache.txt
cmake .. -DLOCALDEPS="BOOST,MPIR,CRYPTOPP"
```

Try building instruction for beginners: [retesteth + solidity build](https://github.com/ethereum/retesteth#building-instructions-for-beginners)


# Usage
Wiki: https://github.com/ethereum/retesteth/wiki
Requires to have a client installed on your system. Read the wiki page on detailed instruction on how to configure your client to work with `retesteth`
https://github.com/ethereum/retesteth/wiki/Add-client-configuration-to-Retesteth
```bash
./retesteth -t GeneralStateTests -- --testpath "your path to ethereum/tests repo"
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
