cmake_minimum_required(VERSION 3.5.1)
include("./cmake/Hunter/HunterGate.cmake")

set(HUNTER_CONFIGURATION_TYPES Release)
set(HUNTER_JOBS_NUMBER 4)
set(HUNTER_CACHE_SERVERS "https://github.com/ethereum/hunter-cache")
HunterGate(
	URL "https://github.com/ruslo/hunter/archive/v0.19.232.tar.gz"
	SHA1 "a412c45fe4c5a72fed386f62dd8d753bd4fd3d11"
	LOCAL
)

project(retesteth)
set(CMAKE_CXX_STANDARD 11) # C++11...
set(CMAKE_CXX_STANDARD_REQUIRED ON) #...is required...
set(CMAKE_CXX_EXTENSIONS OFF) #...without compiler extensions like gnu++11

hunter_add_package(Boost COMPONENTS filesystem program_options system thread)
find_package(Boost CONFIG REQUIRED filesystem program_options system thread)
hunter_add_package(jsoncpp)
find_package(jsoncpp CONFIG REQUIRED)

#Cmake files configuration
set(ETH_CMAKE_DIR "${CMAKE_CURRENT_LIST_DIR}/cmake" CACHE PATH "The path to the cmake directory")
list(APPEND CMAKE_MODULE_PATH ${ETH_CMAKE_DIR})
include(EthOptions)
include(EthDependencies)
configure_project()

add_subdirectory(devcore)
add_subdirectory(testeth)

