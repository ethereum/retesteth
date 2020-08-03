# Getting Started

### Ori Pomerantz, qbzzt1@gmail.com

## Configure the Tests on Docker

The easiest way to start running tests on Ethereum node software (a.k.a. Ethereum clients) is to run `retesteth` inside a Docker container. 
These directions are written using Debian Linux on Google Cloud Platform, but should work with minor changes on any other version of Linux
running anywhere else with an Internet connection.

1. Install Docker. You may need to reboot afterwards to get the latest kernel version.
~~~
sudo apt install -y wget docker docker.io
~~~
2. Download the retesteth docker image:
~~~
wget http://retesteth.ethdevops.io/dretesteth.tar
~~~
3. Load the docker image:
~~~
sudo docker load -i dretesteth.tar 
~~~
4. Download the `dretesteth.sh` script:
~~~
wget https://raw.githubusercontent.com/ethereum/retesteth/develop/dretesteth.sh
chmod +x dretesteth.sh
~~~
> **Note:** At present it is necessary to download it from the `develop` branch. Eventually 
> `dretesteth.sh` will be added to `master`.
5. Download the tests:
~~~
git clone https://github.com/ethereum/tests.git
~~~
6. Run a test. This has two purposes:
   - Create the `retesteth` configuration directories in `~/tests/config`, where we can modify them.
   - A sanity check (that you can run tests successfully).
~~~
sudo ./dretesteth.sh -t GeneralStateTests/stBadOpcode -- --testpath ~/tests --datadir /tests/config
~~~
The output should be similar to:
~~~
Running 1 test case...
Running tests using path: /tests
Active client configurations: 't8ntool '
Running tests for config 'Ethereum GO on StateTool' 2
Test Case "stBadOpcode": 
100%

*** No errors detected
*** Total Tests Run: 1
~~~

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

If you look inside `~/tests/config`, you'll see 

### Your Client Runs Inside the Docker

1. Move the client into `~/tests`
2. Modify the appropriate configuration
3. Run tests

### Your Client Runs Outside the Docker

1. In the configuration remove the `start.sh` and `stop.sh`
2. Modify the host in the `socketAddress` to the appropriate remote address
