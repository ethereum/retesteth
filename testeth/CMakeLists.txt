file(GLOB_RECURSE sources "*.h" "*.cpp")
add_executable(${PROJECT_NAME} ${sources})

target_link_libraries(${PROJECT_NAME} PUBLIC Boost::filesystem Boost::program_options Boost::system Boost::thread jsoncpp_lib_static devcore)
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
