#!/bin/bash
BUILDPATH="/data"
WEBPATH="/data/web"
HOMEPATH="/home/ubuntu"
ARGCLIENT=$1
ARGBUILD=$2


# Prepare build info
echo "Fetch tests: "
cd $BUILDPATH/tests
git reset --hard HEAD~1
git fetch origin
git checkout develop
#git checkout headers
git pull
git submodule update --init
TEST_HEAD=$(git rev-parse HEAD | cut -c1-7)


if [ "$ARGBUILD" != "norebuild" ]; then

echo "Fetch retesteth: "
cd $BUILDPATH/retesteth
git reset --hard HEAD~100
git fetch origin
git checkout develop
#git checkout 6d1b4c2 #transaction cache
git pull
rm -r ./build
rm -r $HOMEPATH/.retesteth/
RETESTETH_HEAD=$(git rev-parse HEAD | cut -c1-7)
echo "Build retesteth: "
mkdir build
cd build && cmake .. -DCMAKE_BUILD_TYPE=Release -DSANITIZE="" -DUNITTESTS="1" && make
#cd build && cmake .. -DCMAKE_BUILD_TYPE=Debug -DSANITIZE=thread && make
#cd build && cmake .. -DCMAKE_BUILD_TYPE=Debug -DSANITIZE=address && make

if [ "$1" = "geth" ] || [ "$1" = "t8ntool" ] || [ -z "$1" ]; then
    echo "Fetch go-ethereum: "
    cd $BUILDPATH/go-ethereum
    git reset --hard HEAD~100
    git fetch origin
    git checkout master
 #   git checkout 60e30a9
    git pull
    rm ./build/bin/geth
    rm ./build/bin/evm
    GETH_HEAD=$(git rev-parse HEAD | cut -c1-7)
    GETHTool_HEAD=$(git rev-parse HEAD | cut -c1-7)
    echo "Build go-ethereum: "
    make all
fi

if [ "$1" = "besu" ] || [ -z "$1" ]; then
    echo "Fetch besu: "
    cd $BUILDPATH/besu
    git reset --hard HEAD~1
    git fetch origin
    git checkout main
#    git fetch shemnon
#    git checkout shemnon/t8n
    git pull
    rm ./build/install/besu/bin/besu
    rm ./ethereum/evmtool/build/install/evmtool/bin/evm
    BESU_HEAD=$(git rev-parse HEAD | cut -c1-7)
    echo "Build besu: "
    # export JAVA_HOME=/usr/lib/jvm/java-11-openjdk-amd64
    # export JAVA_HOME=/usr/lib/jvm/java-14-oracle
    export JAVA_HOME=/usr/lib/jvm/java-17-oracle
    ./gradlew clean distZip -x test
    ./gradlew build
    #./gradlew integrationTest
    #./gradlew installDist
    ./gradlew ethereum:evmtool:installDist
    killall java
fi

if [ "$1" = "ethereumjs" ] || [ -z "$1" ]; then
    echo "Fetch ethereumjs: "
    nvm use 16
    cd $BUILDPATH/ethereumjs-monorepo
    git reset --hard HEAD~1
    git fetch origin
    git checkout master
    git pull
    ETHEREUMJS_HEAD=$(git rev-parse HEAD | cut -c1-7)
    echo "Build ethereumjs: "
    nvm use 19
    npm i
    npm run build --workspaces
fi

if [ "$1" = "nimbus" ] || [ -z "$1" ]; then
    echo "Fetch nimbus: "
    cd $BUILDPATH/nimbus-eth1
    rm -r vendor
    git reset --hard HEAD~1
    git fetch origin
    git checkout master
    git pull
    git submodule update --recursive
    rm ./tools/t8n/t8n
    NIMBUS_HEAD=$(git rev-parse HEAD | cut -c1-7)
    echo "Build nimbus: "
    make t8n -j2
fi

if [ "$1" = "nethermind" ] || [ "$1" = "nethermindt8ntool" ] || [ -z "$1" ]; then
    echo "Fetch nethermind: "
    cd $BUILDPATH/nethermind
    git reset --hard HEAD~1
    git fetch origin
    git checkout feature/t8n-test
    git pull
    rm ./src/Nethermind/artifacts
    rm ./tools/Evm/Evm/bin
    rm ./tools/Evm/Evm/obj
    NETHERMIND_HEAD=$(git rev-parse HEAD | cut -c1-7)
    echo "Build nethermind: "
    make all
fi

#if [ "$1" = "oewrap" ] || [ -z "$1" ]; then
#    echo "Fetch open-ethereum: "
#    cd $BUILDPATH/openethereum
#    git reset --hard HEAD~1
#    git fetch origin
#    git checkout main
#    git pull
#    rm -r  ./target/release
#    OEWR_HEAD=$(git rev-parse HEAD | cut -c1-7)
#    echo "Build open-ethereum: "
#    cargo build --release
#    sleep 10
#    cargo build --release -p evmbin
#fi

if [ "$1" = "aleth" ] || [ "$1" = "testeth" ] || [ -z "$1" ]; then
    echo "Fetch aleth: "
    cd $BUILDPATH/aleth/build
#    git reset --hard HEAD~1
#    git fetch winsvega
#    git checkout winsvega/updatetests2
#    git pull
#    rm ./aleth/aleth
#    rm ./test/testeth
    ALETH_HEAD=$(git rev-parse HEAD | cut -c1-7)
#    echo "Build aleth: "
#    cmake .. -DCMAKE_BUILD_TYPE=Release
#    make
fi
fi #ARGREBUILD

TIMESTAMP=$(date +%Y-%m-%d)
cleanMem() {
 sleep 10
 killall geth
 killall besu
 killall retesteth
 killall aleth
 killall testeth
 killall openethereum-evm
 killall -9 java
 killall python
 killall node
 killall nimbus
 killall nethermind
 sleep 10
}

threads=4
client="geth"
runCmd() {
 echo "Running $client $CMD"
 local TIMES=$(date +%s)
 local logdir="$WEBPATH/results/log"
 local rellogdir="./results/log/"
 local logname="$TIMESTAMP-$TIMES-$client.txt"
 local memlogname="$TIMESTAMP-$TIMES-MEM$client.txt"
 local memlogpic="$TIMESTAMP-$TIMES-$client.png"
 cleanMem
 local headinfo=""
 local clientcfg="--clients $client"
 for (( i=0; i<$threads; i++ ))
 do
   if [ "$client" = "geth" ]; then
     headinfo="Geth: #$GETH_HEAD"
     clientcfg="--clients default"
   fi
   if [ "$client" = "besu" ]; then
     headinfo="Besu: #$BESU_HEAD"
   fi
   if [ "$client" = "aleth" ] || [ "$client" = "testeth" ]; then
     headinfo="Aleth: #$ALETH_HEAD"
   fi
   if [ "$client" = "t8ntool" ]; then
     headinfo="GethTool: #$GETHTool_HEAD"
   fi
   if [ "$client" = "t8ntooleip" ]; then
     headinfo="GethTool: #$GETHTool_HEAD"
   fi
   if [ "$client" = "ethereumjs" ]; then
     headinfo="EthJSTool: #$ETHEREUMJS_HEAD"
   fi
   if [ "$client" = "oewrap" ]; then
     headinfo="OEWrapper: #$OEWR_HEAD"
   fi
   if [ "$client" = "nimbus" ]; then
     headinfo="Nimbus: #$NIMBUS_HEAD"
   fi
   if [ "$client" = "nethermind" ]; then
     headinfo="Nethermind: #NETHERMIND_HEAD"
   fi
   if [ "$client" = "retesteth" ]; then
     clientcfg=""
   fi
   if [ "$client" = "dretesteth" ]; then
     clientcfg=""
   fi
 done
 sleep 20

 if [ "$client" = "testeth" ]; then
    echo "$BUILDPATH/aleth/build/test/testeth $CMD --exectimelog --testpath $BUILDPATH/tests &> $logdir/$logname"
    $BUILDPATH/aleth/build/test/testeth $CMD --exectimelog --testpath $BUILDPATH/tests &> $logdir/$logname
 elif [ "$client" = "retesteth" ]; then  
    $BUILDPATH/retesteth/build/retesteth/retesteth $CMD --nologcolor --exectimelog $clientcfg --testpath $BUILDPATH/tests &> $logdir/$logname        
 elif [ "$client" = "dretesteth" ]; then
    $BUILDPATH/retesteth/dretesteth.sh $CMD --nologcolor --exectimelog $clientcfg --testpath $BUILDPATH/tests &> $logdir/$logname         
 else
    echo "$BUILDPATH/memlogpid.py $BUILDPATH/retesteth/build/retesteth/retesteth $CMD --nologcolor --exectimelog $clientcfg --testpath $BUILDPATH/tests $logdir/$logname $logdir/$memlogname"
    python $BUILDPATH/memlogpid.py "$BUILDPATH/retesteth/build/retesteth/retesteth $CMD --nologcolor --exectimelog $clientcfg --testpath $BUILDPATH/tests" $logdir/$memlogname $logdir/$logname
    #gnuplot -e "set key textcolor rgb 0xffffff; set title textcolor rgb 0xffffff; set border linecolor rgb 0xffffff;  set terminal png background 0x000000   size 1024,200; set output '$logdir/$memlogpic'; plot '$logdir/$memlogname' title '$memlogname' w linespoints pt 6 lc rgb 0xbfff00"
    gnuplot -e "set y2tics textcolor rgbcolor 0x4ac9ff; set ytics nomirror textcolor rgbcolor 0xbfff00; set key textcolor rgb 0xffffff; set title textcolor rgb 0xffffff; set border linecolor rgb 0xffffff;  set terminal png background 0x000000   size 1024,200; set output '$logdir/$memlogpic'; set multiplot; plot '$logdir/$memlogname' using 1:2 title 'mem' axes x1y1 w linespoints pt 1 lc rgb 0xbfff00, '$logdir/$memlogname' using 1:3 title 'cpu' axes x1y2 w linespoints pt 1 lc rgb 0x4ac9ff;"
 fi

 PCMD=$CMD
 if [ ${#CMD} -gt 50 ]; then
   PCMD="${CMD:0:47}..."
 fi
 NAME="$TIMESTAMP <b>$client $PCMD</b> (Test: #$TEST_HEAD, $headinfo, Retesteth: #$RETESTETH_HEAD) Log: (<a href='$rellogdir/$logname'>$logname</a>)"
 cleanMem

 local pass="<font class='passed'>Passed</font>"
 local fail="<font class='failed'>Failed</font>"
 local ttime=$(grep "Total Time" $logdir/$logname)
 local execTime=""
 if [[ ! -z "$ttime" ]]; then
     execTime="</br><font style='font-size:20px'>$ttime</font>"
 fi

 if grep -q "No errors" $logdir/$logname
 then
   if grep -q "Finishing retesteth run" $logdir/$logname
   then
       echo "$fail $NAME $execTime" >> $WEBPATH/results/$logname
   else
       echo "$pass $NAME $execTime" >> $WEBPATH/results/$logname
   fi
 else
   echo "$fail $NAME $execTime" >> $WEBPATH/results/$logname
 fi
 sleep 5
}

#Comands
cname=$1
arg2=$2
if [ "$ARGBUILD" = "norebuild" ]; then
  arg2=""
fi

if [ "$cname" = "retesteth" ] || [ -z "$cname" ]; then
    threads=1
    client="retesteth"
    CMD="-t UnitTests --"
    runCmd

    CMD="-t GeneralStateTests -- --all -j2 --checkhash"
    runCmd
    CMD="-t BlockchainTests -- --all -j2 --checkhash"
    runCmd
    CMD="-t BCGeneralStateTests -- --all -j2 --checkhash"
    runCmd
    CMD="-t LegacyTests -- --all -j2 --checkhash"
    runCmd
fi

if [ "$cname" = "t8ntool" ] || [ -z "$cname" ]; then
    sleep 10
    threads=2
    client="t8ntool"
    CMD="-t GeneralStateTests -- --all -j$threads $arg2"
    runCmd
    CMD="-t BlockchainTests -- --all -j$threads $arg2"
    runCmd
    CMD="-t TransactionTests -- --all -j$threads $arg2"
    runCmd
    CMD="-t DifficultyTests -- --all -j$threads $arg2"
    runCmd
    if [ "$arg2" != "--filltests" ]; then
        CMD="-t BCGeneralStateTests -- --all -j$threads $arg2"
        runCmd
        CMD="-t LegacyTests -- --all -j$threads $arg2"
        runCmd
    fi
fi

if [ "$cname" = "nethermind" ] || [ -z "$cname" ]; then
    sleep 10
    threads=2
    client="nethermind"
    CMD="-t GeneralStateTests -- --all -j$threads $arg2"
    runCmd
    CMD="-t BlockchainTests -- --all -j$threads $arg2"
    runCmd
    CMD="-t TransactionTests -- --all -j$threads $arg2"
    runCmd
    CMD="-t DifficultyTests -- --all -j$threads $arg2"
    runCmd
    if [ "$arg2" != "--filltests" ]; then
        CMD="-t BCGeneralStateTests -- --all -j$threads $arg2"
        runCmd
        CMD="-t LegacyTests -- --all -j$threads $arg2"
        runCmd
    fi
fi

#if [ "$cname" = "oewrap" ] || [ -z "$cname" ]; then
#    sleep 10
#    threads=1
#    client="oewrap"
#    CMD="-t GeneralStateTests -- --all -j$threads $arg2"
#    runCmd
#    CMD="-t BlockchainTests -- --all -j$threads $arg2"
#    runCmd
#    if [ "$arg2" != "--filltests" ]; then
#        CMD="-t LegacyTests -- --all -j$threads $arg2"
#        runCmd
#    fi
#fi


#if [ "$cname" = "besu" ] || [ -z "$cname" ]; then
#    sleep 10
#    threads=1
#    client="besu"
#    CMD="-t GeneralStateTests -- --all -j$threads $arg2"
#    runCmd
#    CMD="-t BlockchainTests -- --all -j$threads $arg2"
#    runCmd
#    if [ "$arg2" != "--filltests" ]; then
#        CMD="-t LegacyTests/Constantinople -- --all -j$threads $arg2"
#        runCmd
#    fi
#fi

if [ "$cname" = "nimbus" ] || [ -z "$cname" ]; then
    sleep 10
    threads=2
    client="nimbus"
    CMD="-t GeneralStateTests -- --all -j$threads $arg2"
    runCmd
    CMD="-t BlockchainTests -- --all -j$threads $arg2"
    runCmd
    if [ "$arg2" != "--filltests" ]; then
        CMD="-t LegacyTests/Constantinople -- --all -j$threads $arg2"
        runCmd
    fi
fi


if [ "$cname" = "testeth" ] || [ -z "$cname" ]; then
    threads=1
    client="testeth"
    CMD="-t RlpTests -- --all $arg2"
    runCmd
    if [ "$arg2" != "--filltests" ]; then
        CMD="-t LegacyTests -- --all $arg2"
        runCmd
    fi
fi


if [ "$cname" = "ethereumjs" ] || [ -z "$cname" ]; then
    sleep 10
    echo "NODE:"
    whoami
    nvm use 16
    which node
    node --version
    threads=2
    client="ethereumjs"
    CMD="-t GeneralStateTests -- --all --lowcpu -j$threads $arg2"
    runCmd
#    CMD="-t BlockchainTests -- --all -j$threads $arg2"
#    runCmd
#    if [ "$arg2" != "--filltests" ]; then
#        CMD="-t LegacyTests/Constantinople -- --all --lowcpu -j$threads $arg2"
#        runCmd
#    fi
fi

if [ "$cname" = "dretesteth" ] || [ -z "$cname" ]; then
    sleep 10
    cd $BUILDPATH/retesteth
    ./dretesteth clean
    ./dretesteth build
    sleep 10
    threads=1
    client="dretesteth"
    CMD="-t GeneralStateTests/stExample -- --singletest add11 --clients t8ntool -j$threads $arg2"
    runCmd
    CMD="-t GeneralStateTests/stExample -- --singletest add11 --clients nethermind -j$threads $arg2"
    runCmd
    CMD="-t GeneralStateTests/stExample -- --singletest add11 --clients besu -j$threads $arg2"
    runCmd
    CMD="-t GeneralStateTests/stExample -- --singletest add11 --clients ethereumjs -j$threads $arg2"
    runCmd
    CMD="-t GeneralStateTests/stExample -- --singletest add11 --clients nimbus -j$threads $arg2"
    runCmd
    CMD="-t GeneralStateTests/stExample -- --singletest add11 --clients evmone -j$threads $arg2"
    runCmd
fi

#cp $HOMEPATH/.retesteth/logs/* $WEBPATH/web/logs
rm $HOMEPATH/.retesteth/logs/*
