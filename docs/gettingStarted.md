# Getting Started

## Ori Pomerantz, qbzzt1@gmail.com

The easiest way to start running tests on Ethereum node software (a.k.a. Ethereum clients) is to run `retesteth` inside a Docker container. 
These directions are written using [Ubuntu Linux](https://ubuntu.com/), but should work with minor changes on other versions of Linux.

1. Install `git`:
~~~
sudo apt install -y git
~~~
2. Install Docker. You may need to reboot afterwards to get the latest kernel version.
~~~
sudo apt-get install -y apt-transport-https ca-certificates curl gnupg-agent software-properties-common
~~~
3. Download the `dretesteth.sh` script:
~~~
wget https://raw.githubusercontent.com/ethereum/retesteth/develop/dretesteth.sh
~~~
> **Note:** At present it is necessary to use the `develop` branch. Eventually `dretesteth.sh` will be added to `master`.
> When that happens, it will be possible to remove this step.
4. Clone the `retesteth` repository
~~~
git clone https://github.com/ethereum/retesteth.git
~~~
5. Run the script:
~~~
chmod +x dretesteth.sh
sudo ./dretesteth.sh
~~~

