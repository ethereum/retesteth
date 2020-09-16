buildImage () {
    docker build -t retesteth .
    exit 0
}

case $1 in
    "build")
        buildImage
        ;;
esac


# Parse arguments and intercept --testpath argument for docker
separator=0
testpaths=0
testpath="notfound"
argstring=""
clientsopt=0
for var in "$@"
do
    if [ "$var" = "--" ]; then
        separator=1
        argstring=$argstring" "$var
        continue
    fi
    if [ "$var" = "--help" ]; then
        argstring=$argstring" "$var
        testpath=""
        break;
    fi
    if [ "$var" = "--testpath" ] && [ "$separator" -eq "1" ]; then
        testpaths=1
        continue
    fi
    if [ "$testpaths" -eq "1" ]; then
        testpaths=0
        testpath=$var
        continue
    fi
    if [ "$var" = "--clients" ] && [ "$separator" -eq "1" ]; then
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

docker run -v $testpath:/tests retesteth $argstring --testpath /tests $defaultclient
