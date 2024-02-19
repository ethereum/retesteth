# retesteth

A test generation tool for the test fillers https://github.com/ethereum/tests/tree/develop/src  
Using state transition tool [t8n](https://ethereum-tests.readthedocs.io/en/latest/t8ntool-ref.html)  
More t8n spec docs: https://github.com/ethereum/go-ethereum/tree/master/cmd/evm  
Execution stats: http://retesteth.ethdevops.io/  
Web interface: http://retesteth.ethdevops.io/web/

# Supported clients / evm's
- [geth](https://github.com/ethereum/go-ethereum)   
- [besu](https://github.com/hyperledger/besu)   
- [nimbus](https://github.com/status-im/nimbus-eth1)  
- [evmone](https://github.com/ethereum/evmone.git)  
- [ethereumjs](https://github.com/ethereumjs/ethereumjs-monorepo.git)  
- [coregeth](https://github.com/etclabscore/core-geth) (etc | etctranslate configs)  

# Docker Instructions

1. Download docker scripts: 
   - [`Dockerfile`](https://raw.githubusercontent.com/ethereum/retesteth/develop/Dockerfile)
   - [`dretesteth.sh`](https://raw.githubusercontent.com/ethereum/retesteth/develop/dretesteth.sh)

   To use [the pyspec tf generator]() instead of retesteth in the docker, rename the `dretesteth.sh` to `dtf.sh`.
   
   **Note:** It is best to put these files in their own directory.
   The reason is that any files in the same directory, including subdirectories, will be added to the docker container, slowing things down.

1. To setup the clients required, edit the args in Dockerfile script.
   Setup github repo and branch/commit hash to build from. Leaving an empty `""` field will disable the client build in the Docker

   ```
   ARG BESU_SRC="https://github.com/hyperledger/besu.git"
   ARG PYSPECS_SRC="https://github.com/ethereum/execution-spec-tests"
   ARG ETEREUMJS_SRC="https://github.com/ethereumjs/ethereumjs-monorepo.git"
   ARG RETESTETH_SRC="https://github.com/ethereum/retesteth.git"
   ARG GETH_SRC="https://github.com/ethereum/go-ethereum.git"
   ARG NIMBUS_SRC="https://github.com/status-im/nimbus-eth1.git"
   ARG EVMONE_SRC="https://github.com/ethereum/evmone.git"

   # Leave empty to disable the build, can point to commit hash as well
   ARG BESU="main"
   ARG GETH="master"
   ARG NIMBUS="master"
   ARG ETHEREUMJS="master"
   ARG RETESTETH="develop"
   ARG PYSPECS="main"
   ARG EVMONE="master"
   ```

1. Build docker locally (if building all clients, ~10 gb space required):

   ```
   chmod +x ./dretesteth.sh
   ./dretesteth.sh build
   ./dretesteth.sh install
   ```

1. Edit `dretesteth.sh` and setup local path to the [test](https://github.com/ethereum/tests) repo so not to type `--testpath`.  
   Navigate to the testpath folder test. And use retesteth normally (`dr` will be linked to `dretesteth.sh` after `dretesteth.sh install` command):

   ```
   dr test.json
   dr testFiller.json --filltests --clients besu|evmone|ethereumjs|nimbus|t8ntool(default)
   dr testfolder
   dr -t GeneralStateTest
   dr -t BlockchainTests
   dretesteth.sh --help
   ```

   The command will run retesteth and clients from the docker container but using local tests.
   Useful options (see `--help`):

   ```
   --statediff
   --poststate
   --vmtraceraw | --vmtrace
   --singlenet
   ```

   See the [usage tutorial](https://ethereum-tests.readthedocs.io/en/latest/retesteth-tutorial.html)

# Building locally

## Basic deps
g++-11 / g++-9 required  
Make sure `cmake` version is higher than VERSION 3.9.3, otherwise install `cmake` from a different source

```sh
sudo apt-cache policy cmake
sudo apt-get update
sudo apt-get install git g++ build-essential cmake
```

## Retesteth

```sh
git clone https://github.com/ethereum/retesteth.git
cd retesteth && git checkout develop
mkdir build && cd build
cmake ..
make -j4
```

You should see the successful build files generation result after (`cmake ..` command): 
```
Configuring done
-- Generating done
-- Build files have been written 
```


## Test clients

Setup at least one client (default is `geth`).
Compile required clients locally.
Use [Dockerfile](https://github.com/ethereum/retesteth/blob/develop/Dockerfile) as a hint to setup the clients or visit official page for instructions.

Retesteth is looking for client's t8n aliases in the PATH, copy or link them in the system:
```
cp /geth/build/bin/evm /bin/evm
cp /nimbus/tools/t8n/t8n /bin/evm_nimbus
ln -s /besu/ethereum/evmtool/build/install/evmtool/bin/evm /usr/bin/besuevm
ln -s /evmone/build/bin/evmone-t8n /usr/bin/evmone
```

Setup evn vars: 
```
ETHEREUMJS_PATH env var is required for ethereumjs client
PYSPECS_PATH env var to generate .py test fillers (https://github.com/ethereum/execution-spec-tests)
ETHEREUM_TEST_PATH env to setup default path to the test repo (so not to type --testpath)
```

## LLLC to compile LLL basic code in the test fillers

`lllc` compiles [Lisp Like Language](https://media.consensys.net/an-introduction-to-lll-for-ethereum-smart-contract-development-e26e38ea6c23), an old Ethereum smart contract language that is still in use in tests.

```
apt-get install --yes libboost-filesystem-dev libboost-system-dev libboost-program-options-dev libboost-test-dev
git clone --depth 1 -b master https://github.com/winsvega/solidity.git /solidity
mkdir /build && cd /build
cmake /solidity -DCMAKE_BUILD_TYPE=Release -DLLL=1 && make lllc
cp /build/lllc/lllc /bin/lllc
```

Optionally clean the cache:
```
rm -rf /build /solidity /var/cache/* /root/.hunter/*  
```

## Solidity to compile solidity and yul code in the test fillers
```
wget https://github.com/ethereum/solidity/releases/download/v0.8.17/solc-static-linux
cp solc-static-linux /usr/bin/solc
chmod +x /usr/bin/solc
```

# Installing on MacOS
See https://github.com/ethereum/retesteth/blob/develop/circle.yml file as a hint.  
use `cmake .. -DLOCALDEPS=BOOST` to disable hunter boost autoinstall to use locally installed version of BOOST (if there are issues with boost)

# Contact if any question:
Telegram: @wdimitry
