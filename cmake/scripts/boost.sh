#!/bin/sh
echo "Build dependencies: "
echo "BOOST 1.79"

echo "working dir: "$(pwd)
if [ ! -d "dependecies" ]; then
    mkdir dependecies
    cd dependecies
    mkdir boost
    cd boost
else
    cd "dependecies/boost"
fi


FILE=boost_1_79_0.tar.gz
if test -f "$FILE"; then
    echo "$FILE exists, use downloaded version..."
else
    curl -L --output boost_1_79_0.tar.gz "https://boostorg.jfrog.io/artifactory/main/release/1.79.0/source/boost_1_79_0.tar.gz"
fi

if [ ! -d "boost_1_79_0" ]; then
    echo "unpack $FILE..."
    tar -xf $FILE
    cd boost_1_79_0
    mkdir build
    echo "configure boost: "
    echo "bootstrap.sh --prefix=$(pwd)/build"
    ./bootstrap.sh --prefix=$(pwd)/build
    libs=""
    for var in "$@"
    do
        libs=$libs" --with-$var"
    done
    ./b2 $libs link=static
else
    echo "found boost previous build in $(pwd)/boost_1_79_0"
fi




