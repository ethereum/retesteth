# Define the path one time
testpath="notfound"

installImage() {
    SCRIPT=$(readlink -f "$0")              # Get the absolute path of the script
    SCRIPT_NAME=$(basename "$SCRIPT")       # Get the name of the script without the path
    SCRIPT_NAME_LINK="${SCRIPT_NAME::-3}"

    if [ "$SCRIPT" != "/usr/bin/$SCRIPT_NAME" ]; then
      sudo ln -s "$SCRIPT" "/usr/bin/$SCRIPT_NAME_LINK"
      echo "Added link /usr/bin/$SCRIPT_NAME_LINK >> $SCRIPT"
      if [ "$SCRIPT_NAME" = "dretesteth.sh" ]; then
        sudo ln -s "$SCRIPT" "/usr/bin/dr"
        echo "Added link /usr/bin/dr >> $SCRIPT"
      fi
    fi
    exit 0
}

buildImage () {
    docker build -t retesteth .
    exit 0
}

rebuildImage () {
    docker build --no-cache -t retesteth .
    exit 0
}

cleanDocker () {
    # Ask user a confirmation
    read -p "This will clean all docker containers and docker system, do you want to proceed? (y|n) " -n 1 -r
    echo    # (optional) move to a new line
    if [[ $REPLY =~ ^[Yy]$ ]]
    then
        docker rmi retesteth:latest
        docker image rm retesteth
        docker image prune -a -f
        docker container prune -f
        docker volume prune -f
        docker image rm retesteth
        docker rmi retesteth:latest
        docker system prune -f
    fi
    exit 0
}

lllc () {
    testpath=""
    testpaths=0
    argstring=""
    for var in "$@"
    do
        if [ "$var" = "--testpath" ]; then
            testpaths=1
            continue
        fi
        if [ "$testpaths" -eq "1" ]; then
            testpaths=0
            testpath=$var
            continue
        fi
        argstring=$argstring" "$var
    done
    if [ -z $testpath ]; then
        echo "Provide --testpath var'to mount into docker '/test'"
        exit 1
    fi
    docker run --entrypoint /bin/lllc -it -v $testpath:/tests -w /tests retesteth $argstring
    exit 0
}


case $1 in
    "build")
        buildImage
        ;;
    "rebuild")
        rebuildImage
        ;;
    "install")
        installImage
        ;;
    "clean")
        cleanDocker
        ;;
    "lllc")
        lllc $2 $3 $4 $5 $6 $7 $8
        ;;
esac


# Pyspecs
SCRIPT=$(readlink -f "$0")
SCRIPT_NAME=$(basename "$SCRIPT")
if [ "$SCRIPT_NAME" = "dtf.sh" ]; then
    clientsopt=0
    testpaths=0
    argstring=""
    for var in "$@"
    do
        if [ "$var" = "--clients" ]; then
            clientsopt=1
            continue
        fi
        if [ "$clientsopt" -eq "1" ]; then
            clientsopt=0
            binpath=""
            if [ "$var" = "geth" ]; then
                binpath="/bin/evm"
            fi
            if [ "$var" = "nimbus" ]; then
                binpath="/bin/evm_nimbus"
            fi
            if [ "$var" = "besu" ]; then
                binpath="/usr/bin/besuevm"
            fi
            if [ "$var" = "ethereumjs" ]; then
                echo "ethereumjs no supported yet"
                binpath=""
                exit 1
            fi
            argstring=$argstring" --evm-bin "$binpath
            continue
        fi
        if [ "$var" = "--testpath" ]; then
            testpaths=1
            continue
        fi
        if [ "$testpaths" -eq "1" ]; then
            testpaths=0
            testpath=$var
            continue
        fi

        if [[ "$var" == *"--filler-path"* ]] || [[ "$var" == *"--output"* ]]; then
            corrected=${var/=/=/tests/}
            argstring=$argstring" "$corrected
            continue
        fi
        argstring=$argstring" "$var
    done

    if [ "$testpath" = "notfound" ]; then
       echo "Please set up the --testpath option! (ex: './dtf.sh --testpath /data/execution-spec-tests')"
       exit 1
    fi

    if [[ "$cwd" == "$testpath"* ]]; then
      dockerwdir="/tests${cwd/$testpath/}"
    else
      dockerwdir="/"
    fi

    docker run --entrypoint /usr/bin/tfinit.sh -it -w $dockerwdir -e BESU_PATH=/besu -e ETHEREUMJS_PATH=/ethereumjs -e PYSPECS_PATH=/execution-spec-tests -e PYT8N_PATH=/pyt8n -v $testpath:/tests retesteth $argstring
    exit 0
fi



# Parse arguments and intercept --testpath argument for docker
testpaths=0
argstring=""
clientsopt=0
helpversion=0
cwd=$(pwd)
for var in "$@"
do
    if [ "$var" = "--" ]; then
        argstring=$argstring" "$var
        continue
    fi
    if [ "$var" = "--help" ] || [ "$var" = "--version" ]; then
        argstring=$argstring" "$var
        testpath=""
        helpversion=1
        break;
    fi
    if [ "$var" = "--testpath" ]; then
        testpaths=1
        continue
    fi
    if [ "$testpaths" -eq "1" ]; then
        testpaths=0
        testpath=$var
        continue
    fi
    if [ "$var" = "--clients" ]; then
        clientsopt=1
    fi
    argstring=$argstring" "$var
done

defaultclient="--clients t8ntool"
if [ "$clientsopt" -eq "1" ]; then
   defaultclient=""
fi

if [ "$testpath" = "notfound" ]; then
   echo "Please set up the --testpath option! (ex: './dretesteth.sh -t GeneralStateTests/stExample -- --testpath /data/tests')"
   exit 1
fi

if [[ "$cwd" == "$testpath"* ]]; then
  dockerwdir="/tests${cwd/$testpath/}"
else
  dockerwdir="/"
fi

if [ "$helpversion" -eq 1 ]; then
    docker run retesteth $argstring
else
    docker run -w $dockerwdir -e BESU_PATH=/besu -e ETHEREUMJS_PATH=/ethereumjs -e PYSPECS_PATH=/execution-spec-tests -e PYT8N_PATH=/pyt8n -v $testpath:/tests retesteth $argstring --testpath /tests $defaultclient
fi
