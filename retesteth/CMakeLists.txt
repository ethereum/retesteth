set(timeout 540)  # The timeout in seconds for individual tests.
file(GLOB_RECURSE sources "*.h" "*.cpp")

if(HUNTER_ENABLED)
    # Find Python executable,
    # prefer version 3 that has requests module included.
    set(Python_ADDITIONAL_VERSIONS 3)
    find_package(PythonInterp)
    if(PYTHONINTERP_FOUND)
        hunter_gate_self(
            "${HUNTER_CACHED_ROOT}"
            "${HUNTER_VERSION}"
            "${HUNTER_SHA1}"
            hunter_dir
            )
        set(hunter_upload_script "${hunter_dir}/maintenance/upload-cache-to-github.py")
        set(hunter_cache_dir "${HUNTER_GATE_ROOT}/_Base/Cache")
        set(hunter_tmp_dir "${HUNTER_GATE_ROOT}/tmp")
        add_custom_target(
            hunter_upload_cache
            ${PYTHON_EXECUTABLE} ${hunter_upload_script}
            --username hunter-cache-bot
            --repo-owner ethereum
            --repo hunter-cache
            --cache-dir ${hunter_cache_dir}
            --temp-dir ${hunter_tmp_dir}
            )
    endif()
endif()

# search for test names and create ctest tests
set(excludeSuites jsonrpc \"customTestSuite\")
set(allSuites jsonrpc)
set(allTests "")
foreach(file ${sources})
    file(STRINGS ${file} test_list_raw REGEX "BOOST_.*TEST_(SUITE|CASE|SUITE_END)")
    set(TestSuite "DEFAULT")
    set(TestSuitePath "")
    foreach(test_raw ${test_list_raw})
        string(REGEX REPLACE ".*TEST_(SUITE|CASE)\\(([^ ,\\)]*).*" "\\1 \\2" test ${test_raw})

        if(test MATCHES "^SUITE .*")
            string(SUBSTRING ${test} 6 -1 TestSuite)
            if (";${excludeSuites};" MATCHES ";${TestSuite};")
                continue()
            endif()
            set(TestSuitePath "${TestSuitePath}/${TestSuite}")
            string(SUBSTRING ${TestSuitePath} 1 -1 TestSuitePathFixed)

            if(FASTCTEST)
                if (NOT ";${allSuites};" MATCHES ";${TestSuite};")
                    string(SUBSTRING ${TestSuitePath} 1 -1 TestSuitePathFixed)
                    list(APPEND allSuites ${TestSuite})
                    separate_arguments(TESTETH_ARGS)
                    set(TestEthArgs -t ${TestSuitePathFixed} -- ${TESTETH_ARGS})
                    add_test(NAME ${TestSuitePathFixed} WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/retesteth COMMAND retesteth ${TestEthArgs})
                    set_tests_properties(${TestSuitePathFixed} PROPERTIES TIMEOUT ${timeout})
                endif()
            endif()
            set(allTests "${allTests}   \"${TestSuitePathFixed}\",\n")
        elseif(test MATCHES "^CASE .*")
            #skip all test cases for the testSuite that is being skipped
            if (";${excludeSuites};" MATCHES ";${TestSuite};")
                continue()
            endif()
            if(NOT FASTCTEST)
                if(NOT test MATCHES "^CASE &createRandom.*")
                    string(SUBSTRING ${test} 5 -1 TestCase)
                    string(SUBSTRING ${TestSuitePath} 1 -1 TestSuitePathFixed)
                    separate_arguments(TESTETH_ARGS)
                    set(TestEthArgs -t ${TestSuitePathFixed}/${TestCase} -- ${TESTETH_ARGS})
                    add_test(NAME ${TestSuitePathFixed}/${TestCase} WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/retesteth COMMAND retesteth ${TestEthArgs})
                    set_tests_properties(${TestSuitePathFixed}/${TestCase} PROPERTIES TIMEOUT ${timeout})
                    set(allTests "${allTests}   \"${TestSuitePathFixed}/${TestCase}\",\n")
                endif()
            endif()
        elseif (";${test_raw};" MATCHES "BOOST_AUTO_TEST_SUITE_END()")
            #skip all test cases for the testSuite that is being skipped
            if (";${excludeSuites};" MATCHES ";${TestSuite};")
                continue()
            endif()
            #encountered SUITE_END block. remove one test suite from the suite path.
            string(FIND ${TestSuitePath} "/" Position REVERSE)
            string(SUBSTRING ${TestSuitePath} 0 ${Position} TestSuitePath)
        endif()
    endforeach(test_raw)
endforeach(file)
enable_testing()

#Save allTests into a variable in include file
configure_file(testSuites/AllTestNames.h.in ${PROJECT_BINARY_DIR}/AllTestNames.h)

add_executable(${PROJECT_NAME} ${sources})

if (JSONCPP)
    add_definitions(-DJSONCPP)
    target_link_libraries(${PROJECT_NAME} PUBLIC ${Boost_LIBRARIES} jsoncpp_lib_static devcore devcrypto dataobj ${CRYPTOPP_LINK} CURL::libcurl)
else()
    target_link_libraries(${PROJECT_NAME} PUBLIC ${Boost_LIBRARIES} devcore devcrypto dataobj ${CRYPTOPP_LINK} CURL::libcurl)
endif()
target_include_directories(${PROJECT_NAME} PRIVATE "../")
target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_BINARY_DIR})
target_include_directories(${PROJECT_NAME} PRIVATE "../retesteth")

if(("${CMAKE_BUILD_TYPE}" STREQUAL "Debug") OR ("${CMAKE_BUILD_TYPE}" STREQUAL "RelWithDebInfo"))
    add_definitions(-DDEBUG)
endif()
