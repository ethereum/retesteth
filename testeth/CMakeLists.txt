file(GLOB_RECURSE sources "*.h" "*.cpp")
add_executable(${PROJECT_NAME} ${sources})

target_link_libraries(${PROJECT_NAME} PUBLIC Boost::filesystem Boost::program_options Boost::system Boost::thread jsoncpp_lib_static devcore)
target_include_directories(${PROJECT_NAME} PRIVATE "../")

