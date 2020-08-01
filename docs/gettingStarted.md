# Getting Started

## Ori Pomerantz, qbzzt1@gmail.com

The easiest way to start running tests on Ethereum node software (a.k.a. Ethereum clients) is to run `retesteth` inside a Docker container. 
These directions are written using [Ubuntu Linux](https://ubuntu.com/), but should work with minor changes on other versions of Linux.

1. Install developer tools:
~~~
sudo apt install -y git g++ build-essential cmake
~~~
2. Install Docker. You may need to reboot afterwards to get the latest kernel version.
~~~
sudo apt-get install -y apt-transport-https ca-certificates curl gnupg-agent software-properties-common
~~~
3. Download the `dretesteth.sh` script:
~~~
wget https://raw.githubusercontent.com/ethereum/retesteth/develop/dretesteth.sh
~~~
> **Note:** At present it is necessary to download it from the `develop` branch. Eventually 
> `dretesteth.sh` will be added to `master`.
> When that happens, it will be possible to remove this step.
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
> **Note:** This is a slow process. On my `g1-small` GCP instance it took about an hour, mostly . 
6. Create the directory for the tests:
~~~
cd ~
mkdir etherTests
~~~
7. Create the docker image. When asked, enter your geographic area. Note that the US is on the top level menu as a different entity than America.
~~~
cd retesteth
chmod +x ../dretesteth.sh
sudo ../dretesteth.sh build build --testpath ~/etherTests
~~~

