file(GLOB sources "*.cpp")
file(GLOB headers "*.h")

add_library(devcore ${sources} ${headers})

#add_dependencies(devcore BuildInfo.h)

# Needed to prevent including system-level boost headers:
target_include_directories(devcore SYSTEM PUBLIC ${Boost_INCLUDE_DIR})

target_link_libraries(devcore ${Boost_LIBRARIES})

#find_package(LevelDB)
#target_include_directories(devcore SYSTEM PUBLIC ${LEVELDB_INCLUDE_DIRS})
#target_link_libraries(devcore ${LEVELDB_LIBRARIES})

