file(GLOB sources "*.cpp")
file(GLOB headers "*.h")

add_library(dataobj ${sources} ${headers})
target_include_directories(dataobj SYSTEM PRIVATE "../")
target_link_libraries(dataobj PRIVATE yaml-cpp)

#add_dependencies(devcore BuildInfo.h)

# Needed to prevent including system-level boost headers:
#target_include_directories(devcore SYSTEM PUBLIC ${Boost_INCLUDE_DIR})



#find_package(LevelDB)
#target_include_directories(devcore SYSTEM PUBLIC ${LEVELDB_INCLUDE_DIRS})
#target_link_libraries(devcore ${LEVELDB_LIBRARIES})

