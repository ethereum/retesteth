file(GLOB sources "*.cpp")
file(GLOB headers "*.h")

add_library(dataobj ${sources} ${headers})
target_include_directories(dataobj SYSTEM PRIVATE "../")
if (DATA_YAML)
    target_link_libraries(dataobj PRIVATE yaml-cpp)
endif()
