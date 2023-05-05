file(GLOB sources "*.cpp")
file(GLOB headers "*.h")

add_library(dataobj ${sources} ${headers})
target_include_directories(dataobj SYSTEM PRIVATE "../")
target_link_libraries(dataobj PRIVATE yaml-cpp)
