docker rmi retesteth:latest
docker image prune -a -f
docker container prune -f
cd /data/retestethdocker
rm /data/retestethdocker/*
wget https://raw.githubusercontent.com/ethereum/retesteth/develop/dretesteth.sh
wget https://raw.githubusercontent.com/ethereum/retesteth/develop/Dockerfile
sed -i 's/-b master https:\/\/github.com\/ethereum\/retesteth.git/-b develop https:\/\/github.com\/ethereum\/retesteth.git/g' Dockerfile
chmod +x ./dretesteth.sh
./dretesteth.sh build
rm /data/web/dretesteth.tar
docker save -o /data/web/dretesteth.tar retesteth
chmod +r /data/web/dretesteth.tar

