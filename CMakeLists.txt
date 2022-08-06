
cmake_minimum_required(VERSION 3.9.3)
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/toolchain.cmake)

include("./cmake/Hunter/HunterGate.cmake")

set(HUNTER_CONFIGURATION_TYPES Release)
set(HUNTER_JOBS_NUMBER 4 CACHE STRING "Hunter jobs number")
set(HUNTER_CACHE_SERVERS "https://github.com/ethereum/hunter-cache" CACHE STRING "Hunter cache servers")

HunterGate(
        URL "https://github.com/ruslo/hunter/archive/v0.23.44.tar.gz"
        SHA1 "c4cfcc0cd39fdae8de08f6205b7f34cab4a7ba79"
        LOCAL
)

project(retesteth VERSION 0.2.3)
set(VERSION_SUFFIX "postmerge")

set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_DEBUG_RUNTIME OFF)

find_package(Boost 1.79 COMPONENTS  filesystem program_options system)
IF( NOT Boost_FOUND )
    message("Boost 1.79 not found in system, trying to autoinstall")
    execute_process(COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/cmake/scripts/boost.sh filesystem program_options system)
    set(BOOST_ROOT ${CMAKE_CURRENT_BINARY_DIR}/dependecies/boost/boost_1_79_0/)
    find_package(Boost 1.79 REQUIRED COMPONENTS  filesystem program_options system PATHS ${BOOST_ROOT}/tools/boost_install ${BOOST_ROOT}/stage/lib/cmake)
    set(Boost_INCLUDE_DIR ${CMAKE_CURRENT_BINARY_DIR}/dependecies/boost/boost_1_79_0/)
ENDIF()

message("Boost libdir: `${Boost_LIBRARIES}`")
message("Boost include: `${Boost_INCLUDE_DIR}`")

hunter_add_package(yaml-cpp)
find_package(yaml-cpp CONFIG REQUIRED)
hunter_add_package(cryptopp)
find_package(cryptopp CONFIG REQUIRED)
hunter_add_package(libscrypt)
find_package(libscrypt CONFIG REQUIRED)

if (POLICY CMP0074)
    cmake_policy(SET CMP0074 NEW)   # Use download timestamp for remote objects
endif()
if (POLICY CMP0135)
    cmake_policy(SET CMP0135 NEW)   # Use provided package_ROOT variables
endif()

hunter_add_package(CURL)
find_package(CURL CONFIG REQUIRED)


#Cmake files configuration
set(ETH_CMAKE_DIR "${CMAKE_CURRENT_LIST_DIR}/cmake" CACHE PATH "The path to the cmake directory")
list(APPEND CMAKE_MODULE_PATH ${ETH_CMAKE_DIR})
include(EthOptions)
include(EthDependencies)
include(EthCompilerSettings)

include(ProjectSecp256k1)
include(ProjectLibFF)
configure_project()

if(JSONCPP)
    hunter_add_package(jsoncpp)
    find_package(jsoncpp CONFIG REQUIRED)
endif()

add_subdirectory(libdevcore)
add_subdirectory(libdevcrypto)
add_subdirectory(retesteth)

