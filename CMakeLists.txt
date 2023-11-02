
cmake_minimum_required(VERSION 3.9.3)
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/toolchain.cmake)

include("./cmake/Hunter/HunterGate.cmake")

if ("${CMAKE_BUILD_TYPE}" STREQUAL "")
    set(CMAKE_BUILD_TYPE "Release")
endif()
if (NOT "${CMAKE_BUILD_TYPE}" STREQUAL "Release")
    add_compile_definitions("__DEBUG__")
endif()
if (${UNITTESTS})
    add_compile_definitions("UNITTESTS")
endif()

set(HUNTER_CONFIGURATION_TYPES Release)
set(HUNTER_JOBS_NUMBER 4 CACHE STRING "Hunter jobs number")
set(HUNTER_CACHE_SERVERS "https://github.com/ethereum/hunter-cache" CACHE STRING "Hunter cache servers")

HunterGate(
    URL "https://github.com/cpp-pm/hunter/archive/v0.24.7.tar.gz"
    SHA1 "908ddcb223f25fa155315baae64ce0ab48a38bf7"
    LOCAL
)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

project(retesteth VERSION 0.3.2)
set(VERSION_SUFFIX "cancun")

set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_DEBUG_RUNTIME OFF)

IF (NOT LOCALDEPS MATCHES ".*BOOST")
    hunter_add_package(Boost COMPONENTS filesystem system)
    find_package(Boost 1.79 REQUIRED COMPONENTS  filesystem system)
    #message(STATUS "[retesteth] Will now autoinstall custom dependency: Boost 1.79")
    #execute_process(COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/cmake/scripts/boost.sh filesystem program_options system)
    #set(BOOST_ROOT ${CMAKE_CURRENT_BINARY_DIR}/dependecies/boost/boost_1_79_0/)
    #find_package(Boost 1.79 REQUIRED COMPONENTS  filesystem program_options system PATHS ${BOOST_ROOT}/tools/boost_install ${BOOST_ROOT}/stage/lib/cmake)
    #set(Boost_INCLUDE_DIR ${CMAKE_CURRENT_BINARY_DIR}/dependecies/boost/boost_1_79_0/)
ELSE()
    message(STATUS "[retesteth] LOCALDEPS=BOOST looking for locally installed version")
    find_package(Boost 1.79 REQUIRED COMPONENTS  filesystem system)
ENDIF()
message(STATUS "[retesteth] Boost libdir: `${Boost_LIBRARIES}`")
message(STATUS "[retesteth] Boost include: `${Boost_INCLUDE_DIR}`")

hunter_add_package(yaml-cpp)
find_package(yaml-cpp CONFIG REQUIRED)

# Silence cmake warnings on newer versions
if (POLICY CMP0074)
    cmake_policy(SET CMP0074 NEW)   # Use download timestamp for remote objects
endif()
if (POLICY CMP0135)
    cmake_policy(SET CMP0135 NEW)   # Use provided package_ROOT variables
endif()

IF (NOT LOCALDEPS MATCHES ".*CURL")
    hunter_add_package(CURL)
    find_package(CURL CONFIG REQUIRED)
ELSE()
    message(STATUS "[retesteth] LOCALDEPS=CURL looking for locally installed version")
    find_package(CURL CONFIG REQUIRED)
ENDIF()


#Cmake files configuration
set(ETH_CMAKE_DIR "${CMAKE_CURRENT_LIST_DIR}/cmake" CACHE PATH "The path to the cmake directory")
list(APPEND CMAKE_MODULE_PATH ${ETH_CMAKE_DIR})
include(EthOptions)
include(EthDependencies)
include(EthCompilerSettings)

include(ProjectSecp256k1)
configure_project()

if(JSONCPP)
    hunter_add_package(jsoncpp)
    find_package(jsoncpp CONFIG REQUIRED)
endif()

add_subdirectory(libdevcore)
add_subdirectory(libdevcrypto)
add_subdirectory(libdataobj)
if (${UNITTESTS})
    set(LIBSSZ ssz)
    add_subdirectory(libssz)
endif()
add_subdirectory(retesteth)
