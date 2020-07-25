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
for var in "$@"
do
    if [ "$var" = "--" ]; then
        separator=1
        argstring=$argstring" "$var
        continue
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
    argstring=$argstring" "$var
done

docker run -v $testpath:/tests retesteth $argstring --testpath /tests --clients "t8ntool"
