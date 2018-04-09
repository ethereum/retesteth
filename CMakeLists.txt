cmake_minimum_required(VERSION 3.5.1)
include("./cmake/Hunter/HunterGate.cmake")

set(HUNTER_CONFIGURATION_TYPES Release)
set(HUNTER_JOBS_NUMBER 4)
set(HUNTER_CACHE_SERVERS "https://github.com/ethereum/hunter-cache")
HunterGate(
	URL "https://github.com/ruslo/hunter/archive/v0.20.9.tar.gz"
	SHA1 "8f7cf6d719d05c49224a4ebeba90c6bb90b90f4a"
	LOCAL
)

project(retesteth VERSION 0.0.1)
set(CMAKE_CXX_STANDARD 11) # C++11...
set(CMAKE_CXX_STANDARD_REQUIRED ON) #...is required...
set(CMAKE_CXX_EXTENSIONS OFF) #...without compiler extensions like gnu++11


set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
hunter_add_package(Boost COMPONENTS filesystem program_options system thread)
find_package(Boost CONFIG REQUIRED filesystem program_options system thread)
hunter_add_package(jsoncpp)
find_package(jsoncpp CONFIG REQUIRED)
hunter_add_package(cryptopp)
find_package(cryptopp CONFIG REQUIRED)
hunter_add_package(libscrypt)
find_package(libscrypt CONFIG REQUIRED)


#Cmake files configuration
set(ETH_CMAKE_DIR "${CMAKE_CURRENT_LIST_DIR}/cmake" CACHE PATH "The path to the cmake directory")
list(APPEND CMAKE_MODULE_PATH ${ETH_CMAKE_DIR})
include(EthOptions)
include(EthDependencies)
include(EthCompilerSettings)

include(ProjectSecp256k1)
include(ProjectLibFF)
configure_project()

add_subdirectory(libdevcore)
add_subdirectory(libdevcrypto)
add_subdirectory(retesteth)

