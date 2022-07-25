# cryptopp has very bad CMakeLists.txt config.
# We have to enforce "cross compiling mode" there by setting CMAKE_SYSTEM_VERSION=NO
# to any "false" value.
hunter_config(cryptopp VERSION ${HUNTER_cryptopp_VERSION} CMAKE_ARGS CMAKE_SYSTEM_VERSION=NO)

hunter_config(
    libjson-rpc-cpp
    VERSION ${HUNTER_libjson-rpc-cpp_VERSION}
    CMAKE_ARGS
    UNIX_DOMAIN_SOCKET_SERVER=NO
    UNIX_DOMAIN_SOCKET_CLIENT=NO
    FILE_DESCRIPTOR_SERVER=NO
    FILE_DESCRIPTOR_CLIENT=NO
    TCP_SOCKET_SERVER=NO
    TCP_SOCKET_CLIENT=NO
    HTTP_SERVER=NO
    HTTP_CLIENT=NO
)

#hunter_config(Boost VERSION 1.65.2
#    URL http://retesteth.ethdevops.io/dependencies/boost_1_65_1.tar.bz2
#    SHA1 4a5b0c3c1b1b9a4d6cb6a6cc395e903e76f76720
#)

#hunter_config(Boost VERSION 1.66.2
#    URL https://boostorg.jfrog.io/artifactory/main/release/1.66.0/source/boost_1_66_0.tar.bz2
#    SHA1 b6b284acde2ad7ed49b44e856955d7b1ea4e9459
#)

hunter_config(Boost VERSION 1.72.0
    URL https://boostorg.jfrog.io/artifactory/main/release/1.72.0/source/boost_1_72_0.tar.bz2
    SHA1 88866e4075e12255e7a7189d0b8a686e0b1ee9c1
)
 
#MacOS dirty patch from https://alice-talk.web.cern.ch/t/o2-build-failed-at-boost-due-to-unknown-argument/545
#hunter_config(Boost VERSION 1.66.2
#    URL http://retesteth.ethdevops.io/dependencies/boost_1_66_2.tar.bz2
#    SHA1 4c7439bde45453ee6990d062d0244e567f2a224c
#)

hunter_config(ethash VERSION 0.4.0
    URL https://github.com/chfast/ethash/archive/v0.4.0.tar.gz
    SHA1 fd92ffadff2931877a3b68685dd8c53f0bdec539
)


hunter_config(CURL VERSION ${HUNTER_CURL_VERSION} CMAKE_ARGS HTTP_ONLY=ON CMAKE_USE_OPENSSL=OFF CMAKE_USE_LIBSSH2=OFF CURL_CA_PATH=none)
