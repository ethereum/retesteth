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

hunter_config(ethash VERSION 0.4.0
    URL https://github.com/chfast/ethash/archive/v0.4.0.tar.gz
    SHA1 fd92ffadff2931877a3b68685dd8c53f0bdec539
)

hunter_config(Boost VERSION 1.79.1
    URL "https://archives.boost.io/release/1.79.0/source/boost_1_79_0.tar.bz2"
    SHA1 "31209dcff292bd6a64e5e08ceb3ce44a33615dc0"
)

hunter_config(CURL VERSION ${HUNTER_CURL_VERSION} CMAKE_ARGS HTTP_ONLY=ON CMAKE_USE_OPENSSL=OFF CMAKE_USE_LIBSSH2=OFF CURL_CA_PATH=none)
