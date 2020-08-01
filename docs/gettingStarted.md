# Getting Started

## Ori Pomerantz, qbzzt1@gmail.com

The easiest way to start running tests on Ethereum node software (a.k.a. Ethereum clients) is to run `retesteth` inside a Docker container. 
These directions are written using Debian Linux on Google Cloud Platform, but should work with minor changes on other versions of Linux.

A lot of disk space is required. 10 GB isn't enough.

<!--
1. Install developer tools:
~~~
sudo apt install -y git g++ build-essential cmake
~~~
-->
1. Install Docker. You may need to reboot afterwards to get the latest kernel version.
~~~
sudo apt install -y wget docker docker.io
~~~
2. Download the `dretesteth.sh` script:
~~~
wget https://raw.githubusercontent.com/ethereum/retesteth/develop/dretesteth.sh
~~~
> **Note:** At present it is necessary to download it from the `develop` branch. Eventually 
> `dretesteth.sh` will be added to `master`.
3. Download the `Dockerfile`:
~~~
wget https://raw.githubusercontent.com/ethereum/retesteth/master/Dockerfile
~~~
4. Create the directory for the tests:
~~~
mkdir ~/etherTests
~~~
5. Create the docker image. Note that this is a slow process, on my `g1-small` GCP instance is took about an hour.
~~~
chmod +x dretesteth.sh
sudo ./dretesteth.sh build --testpath ~/etherTests
~~~

<!--
4. Clone the `retesteth` repository:
~~~
git clone https://github.com/ethereum/retesteth.git
~~~
5. Compile `retesteth`:
~~~
cd retesteth
mkdir build
cd build
cmake ..
make
~~~
> **Note:** This is a slow process. On my `g1-small` GCP instance it took about an hour,  mostly in the last two commands. 

--->


