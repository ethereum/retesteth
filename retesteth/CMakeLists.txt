file(GLOB_RECURSE sources "*.h" "*.cpp")
add_executable(${PROJECT_NAME} ${sources})

target_link_libraries(${PROJECT_NAME} PUBLIC Boost::filesystem Boost::program_options Boost::system Boost::thread jsoncpp_lib_static devcore devcrypto cryptopp-static CURL::libcurl)
target_include_directories(${PROJECT_NAME} PRIVATE "../")

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
set(excludeSuites jsonrpc \"customTestSuite\" GeneralStateTests)
set(allSuites jsonrpc)
foreach(file ${sources})
	file(STRINGS ${file} test_list_raw REGEX "BOOST_.*TEST_(SUITE|CASE|SUITE_END)")
	set(TestSuite "DEFAULT")
	set(TestSuitePath "")
	foreach(test_raw ${test_list_raw})
		string(REGEX REPLACE ".*TEST_(SUITE|CASE)\\(([^ ,\\)]*).*" "\\1 \\2" test ${test_raw})

		#skip disabled
		if (";${excludeSuites};" MATCHES ";${TestSuite};")
			continue()
		endif()

		if(test MATCHES "^SUITE .*")

			string(SUBSTRING ${test} 6 -1 TestSuite)
			set(TestSuitePath "${TestSuitePath}/${TestSuite}")

			if(FASTCTEST)
				if (";${excludeSuites};" MATCHES ";${TestSuite};")
					continue()
				endif()
				if (NOT ";${allSuites};" MATCHES ";${TestSuite};")
					string(SUBSTRING ${TestSuitePath} 1 -1 TestSuitePathFixed)
					list(APPEND allSuites ${TestSuite})
					separate_arguments(TESTETH_ARGS)
					set(TestEthArgs -t ${TestSuitePathFixed} -- ${TESTETH_ARGS})
					add_test(NAME ${TestSuitePathFixed} WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/testeth COMMAND retesteth ${TestEthArgs})
				endif()
			endif()
		elseif(test MATCHES "^CASE .*")
			if(NOT FASTCTEST)
				if(NOT test MATCHES "^CASE &createRandom.*")
					string(SUBSTRING ${test} 5 -1 TestCase)
					string(SUBSTRING ${TestSuitePath} 1 -1 TestSuitePathFixed)
					separate_arguments(TESTETH_ARGS)
					set(TestEthArgs -t ${TestSuitePathFixed}/${TestCase} -- ${TESTETH_ARGS})
					add_test(NAME ${TestSuitePathFixed}/${TestCase} WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/testeth COMMAND retesteth ${TestEthArgs})
				endif()
			endif()
		elseif (";${test_raw};" MATCHES "BOOST_AUTO_TEST_SUITE_END()")
			#encountered SUITE_END block. remove one test suite from the suite path.
			string(FIND ${TestSuitePath} "/" Position REVERSE)
			string(SUBSTRING ${TestSuitePath} 0 ${Position} TestSuitePath)
		endif()
	endforeach(test_raw)
endforeach(file)
enable_testing()

