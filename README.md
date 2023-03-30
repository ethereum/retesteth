# retesteth
A test generaion tool for the test fillers https://github.com/ethereum/tests/tree/develop/src  
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

Download docker scripts: 
```
https://github.com/ethereum/retesteth/blob/develop/Dockerfile
https://github.com/ethereum/retesteth/blob/develop/dretesteth.sh
```

Rename the `dretesteth.sh` to `dtf.sh` to use pyspec's tf generator instead of retesteth in the docker.

Setup the clients required, edit the args in Dockerfile script  
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

Build docker locally (if building all ~10 gb space required):
```
chmod +x ./dretesteth.sh
./dretesteth.sh build
./dretesteth.sh install
```

Edit `dretesteth.sh` and setup local path to the [test](https://github.com/ethereum/tests) repo so not to type `--testpath`.  
Navigate to the testpath folder test. And use retesteth normally:

```
dr test.json
dr testFiller.json --filltests --clients besu|evmone|ethereumjs|nimbus|t8ntool(default)
dr testfolder
dr -t GeneralStateTest
dr -t BlockchainTests
dr --help
```

The command will run retesteth and clients from the docker container but using local tests.
Usefull options (see `--help`):
```
--statediff
--poststate
--vmtraceraw | --vmtrace
--singlenet
```

* [Usage tutorial](https://ethereum-tests.readthedocs.io/en/latest/retesteth-tutorial.html)

# Building locally

#### Basic deps
g++-11 / g++-9 required  
Make sure cmake version is higher than VERSION 3.9.3, otherwise install cmake from a different source
```
sudo apt-cache policy cmake
sudo apt-get update
sudo apt-get install git g++ build-essential cmake
```

#### Retesteth
```
git clone git@github.com:ethereum/retesteth.git
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


#### Test clients
Compile required clients locally  
Use [Dockerfile](https://github.com/ethereum/retesteth/blob/develop/Dockerfile) as a hint to setup the clients or visit official page for instructions  
Setup at least one client (default is `geth`)  
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

#### LLLC to compile lllc basic code in the test fillers
```
apt-get install --yes libboost-filesystem-dev libboost-system-dev libboost-program-options-dev libboost-test-dev
git clone --depth 1 -b master https://github.com/winsvega/solidity.git /solidity
mkdir /build && cd /build
cmake /solidity -DCMAKE_BUILD_TYPE=Release -DLLL=1 && make lllc
cp /build/lllc/lllc /bin/lllc
```
Optional clean the cache:
```
rm -rf /build /solidity /var/cache/* /root/.hunter/*  
```

#### Solidity to compile solidity and yul code in the test fillers
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
