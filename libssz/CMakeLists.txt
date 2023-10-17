file(GLOB_RECURSE sources "*.cpp")
file(GLOB_RECURSE headers "*.h")

add_library(ssz ${sources} ${headers})
target_include_directories(ssz SYSTEM PRIVATE "../")
target_link_libraries(ssz PRIVATE dataobj)
