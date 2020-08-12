# Getting Started with Retesteth

### Ori Pomerantz, qbzzt1@gmail.com

## Configure the Tests on Docker

> **Note:** This document is a tutorial. For reference on the `retesteth` options, 
> [look here](https://github.com/ethereum/retesteth/wiki/Retesteth-commands)
The easiest way to start running tests on Ethereum node software (a.k.a. Ethereum clients) is to run `retesteth` inside a Docker container. 
These directions are written using Debian Linux on Google Cloud Platform, but should work with minor changes on any other version of Linux
running anywhere else with an Internet connection.

1. Install Docker. You may need to reboot afterwards to get the latest kernel version.
   ~~~
   sudo apt install -y wget docker docker.io
   ~~~
1. Download the retesteth docker image:
   ~~~
   wget http://retesteth.ethdevops.io/dretesteth.tar
   ~~~
1. Load the docker image:
   ~~~
   sudo docker load -i dretesteth.tar 
   ~~~
1. Download the `dretesteth.sh` script:
   ~~~
   wget https://raw.githubusercontent.com/ethereum/retesteth/develop/dretesteth.sh
   chmod +x dretesteth.sh
   ~~~
   > **Note:** At present it is necessary to download it from the `develop` branch. Eventually 
   > `dretesteth.sh` will be added to `master`.
1. Download the tests:
   ~~~
   git clone --depth 1 --branch develop https://github.com/ethereum/tests.git
   ~~~
1. Run a test. This has two purposes:
   - Create the `retesteth` configuration directories in `~/tests/config`, where we can modify them.
   - A sanity check (that you can run tests successfully).
   ~~~
   sudo ./dretesteth.sh -t GeneralStateTests/stExample -- --testpath ~/tests --datadir /tests/config
   ~~~
   The output should be similar to:
   ~~~
   Running 1 test case...
   Running tests using path: /tests
   Active client configurations: 't8ntool '
   Running tests for config 'Ethereum GO on StateTool' 2
   Test Case "stExample": 
   100%
   *** No errors detected
   *** Total Tests Run: 1
   ~~~
   > **Note:** The `/tests` directory is referenced inside the docker container. It is the same as the `~/tests` directory outside it.   
### How Does This Work?

A [docker](https://www.docker.com/resources/what-container) container is similar to a virtual machine, except that it doesn't run a separate instance of
the operating system inside itself so it takes far less resources. One of the features of docker is that it can mount a directory of the host computer
inside its own file system. The `--testpath` parameter to `dretesteth.sh` tells it what directory to mount, in this case `~/tests` which you just cloned
from github. It mounts it as `/tests` inside the container.

By default the `retesteth` configuration files are in `~/.retesteth`. However, that directory is not accessible to us outside the docker. Instead, we 
use `--datadir /tests/config` to tell it to use (or create) the configuration in what appears to us to be `~/tests/config`, which is easily accessible.

## Test Against Your Client

There is an instance of `geth` inside the docker container that you can run tests
against. However, unless you are specifically developing tests what you want is to
test your client. There are two ways to do this:

1. Put your client, and any prerequisites, inside the docker
1. Keep your client on the outside and connect to it through the network

In either case you need to edit the `retesteth` configuration files. When we ran
the test in the previous section we also created those configuration files in 
`~/tests/config`, but they were created as being owned by root. To change the
configuration files to your own user, run this command:
~~~
sudo find ~/tests/config -exec chown $USER {} \; -print
~~~

If you look inside `~/tests/config`, you'll see a directory for each configured client. 
Typically this directory has these files:

- `config`, which contains the configuration for the client:
  - The communication protocol to use with the client (typically TCP) with the addresses
  - The address(es) to use with that protocol
  - The forks the client supports
  - The exceptions the client can throw, and how `retesteth` should interpret them
- `start.sh`, which starts the client inside the docker image
- `stop.sh` , which stops the client instance(s)
- `genesis`, a directory which includes the genesis blocks for various forks the client 
  supports. If this directory does not exist for a client, it uses the genesis blocks for
  the default client.

[Click here for additional documentation. Warning: Some things have changed 
since it was written](https://github.com/ethereum/retesteth/wiki/Add-client-configuration-to-Retesteth)


### Your Client Runs Inside the Docker

If you want to run your client inside the docker, follow these steps:

1. Move the client into `~/tests`, along with any required infrastructure (virtual machine software, etc). 
   If you just want to test the directions right now, [you can download `geth` here](https://geth.ethereum.org/downloads/).
1. Modify the appropriate `start.sh` to run your version of the client instead. For example, you might
   edit `~/tests/config/geth/start.sh` to replace `geth` with `/tests/geth` in line ten if you put your version of `geth`
   in `~/tests`.
1. Run the tests, adding the `--clients <name of client>` parameter to ensure you're using the correct configuration. For
   example, run this command to run the virtual machine tests on `geth`:
   ~~~
   sudo ./dretesteth.sh -t BlockchainTests/ValidBlocks/VMTests -- --testpath ~/tests --datadir /tests/config --clients geth
   ~~~
   > **Note:** You can't just run all the tests on a client for some reason. Just select the tests for whatever 
   > you have changed.

### Your Client Runs Outside the Docker

If you want to run your client outside the docker, these are the steps to follow. Note that you can either configure the 
client's IP address and port in the `config` file, or specify them as parameters to `dretesteth.sh`.

1. Create a client in `~/tests/config` that doesn't have `start.sh` and `stop.sh`. Typically you would do this by copying an
   existing client, for example:
   ~~~
   mkdir ~/tests/config/gethOutside
   cp ~/tests/config/geth/config ~/tests/config/gethOutside
   ~~~
1. If you want to specify the IP address and port in the `config` file, modify the host in the `socketAddress` to 
   the appropriate remote address. For example,
   ~~~
   {
    "name" : "Ethereum GO on TCP",
    "socketType" : "tcp",
    "socketAddress" : [
        "10.128.0.14:8545"
    ],
    ~~~
1. Make sure that the routing works in both directions (from the docker to the client and from the client back to the docker).
   You may need to configure [network address translation](https://www.slashroot.in/linux-nat-network-address-translation-router-explained).
1. Run your client. Make sure that the client accepts requests that don't come from `localhost`. For example, to run `geth` use:
   ~~~
   geth --http --http.addr 0.0.0.0 retesteth
   ~~~
1. If you specified the IP address and port in the `config` file, run the test the same way you would for a client that runs inside docker:
   ~~~
   sudo ./dretesteth.sh -t BlockchainTests/ValidBlocks/VMTests -- --testpath ~/tests --datadir /tests/config --clients gethOutside
   ~~~
   If you did not, specify those values here:
   ~~~
   sudo ./dretesteth.sh -t BlockchainTests/ValidBlocks/VMTests -- --testpath ~/tests --datadir /tests/config --clients gethOutside --nodes <ip>:<port, 8545 by default>
   ~~~   

## Conclusion

In most cases people don't start their own client from scratch, but modify an existing client. If the existing client is already configured to support
`retesteth`, you should now be able to run tests on a modified version to ensure it still conforms to Ethereum specifications. If you are writing a 
completely new client, you still need to implement the RPC calls that `retesteth` uses and to write the appropriate configuration (`config`, `start.sh`, 
and `stop.sh`) for it.

There are several actions you might want to do with `retesteth` beyond testing a new version of an existing client. Here are links to documentation. Note
that it hasn't been updated in a while, so it may not be accurate.

* **Add configuration for a new client**. To do this you need to 
  [add retesteth support to the client itself](https://github.com/ethereum/retesteth/wiki/RPC-Methods)
  and [create a new config for it](https://github.com/ethereum/retesteth/wiki/Add-client-configuration-to-Retesteth)
* **[Create a state test](https://github.com/ethereum/retesteth/wiki/Creating-a-State-Test-with-retesteth)**
* **Test with a new fork of Ethererum**. New forks usually mean new opcodes. Therefore, you will need a docker with a new 
  version of [lllc](https://lll-docs.readthedocs.io/en/latest/lll_compiler.html).
