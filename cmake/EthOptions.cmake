macro(configure_project)
    # Default to RelWithDebInfo configuration if no configuration is explicitly specified.
    if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
       set(CMAKE_BUILD_TYPE RelWithDebInfo CACHE STRING "Build type on single-configuration generators" FORCE)
    endif()
    set(TESTETH_ARGS "" CACHE STRING "Testeth arguments for ctest tests")

    option(BUILD_SHARED_LIBS "Build project libraries shared" OFF)

    # Features:
    option(FASTCTEST "Enable fast ctest" OFF)
    option(JSONCPP "Enable jsoncpp for .json hash debugging (--showhash)" OFF)

    # components
  
    # CI Builds should provide (for user builds this is totally optional)
    # -DBUILD_NUMBER - A number to identify the current build with. Becomes TWEAK component of project version.
    # -DVERSION_SUFFIX - A string to append to the end of the version string where applicable.
    if (NOT DEFINED BUILD_NUMBER)
        # default is big so that local build is always considered greater
        # and can easily replace CI build for for all platforms if needed.
        # Windows max version component number is 65535
        set(BUILD_NUMBER 65535)
    endif()

    # Suffix like "-rc1" e.t.c. to append to versions wherever needed.
    if (NOT DEFINED VERSION_SUFFIX)
        set(VERSION_SUFFIX "")
    endif()

    include(EthBuildInfo)
    create_build_info()
    print_config()
endmacro()

macro(print_config)
    message("")
    message("------------------------------------------------------------------------")
    message("-- Configuring ${PROJECT_NAME}")
    message("------------------------------------------------------------------------")
    message("-- CMake ${CMAKE_VERSION} (${CMAKE_COMMAND})")
    message("-- CMAKE_BUILD_TYPE Build type                               ${CMAKE_BUILD_TYPE}")
    message("-- TARGET_PLATFORM  Target platform                          ${CMAKE_SYSTEM_NAME}")
    message("-- BUILD_SHARED_LIBS                                         ${BUILD_SHARED_LIBS}")
    message("-- LOCALDEPS        Try to autolocate dependencies           ${LOCALDEPS}")
    message("------------------------------------------------------------------ tests")
    message("-- FASTCTEST        Run only test suites in ctest            ${FASTCTEST}")
    message("-- JSONCPP          Compile with jsoncpp for debug           ${JSONCPP}")
    message("-- TESTETH_ARGS     Testeth arguments in ctest:               ")
    message("                    ${TESTETH_ARGS}")
    message("------------------------------------------------------------------------")
    message("")
endmacro()
