# Getting Started

## Ori Pomerantz, qbzzt1@gmail.com

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
~~~
> **Note:** At present it is necessary to download it from the `develop` branch. Eventually 
> `dretesteth.sh` will be added to `master`.
5. Download the tests:
~~~
git clone https://github.com/ethereum/tests.git
~~~
6. Run a test as a sanity check:
~~~
sudo ./dretesteth.sh -t GeneralStateTests/stBadOpcode -- --testpath ~/tests
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
