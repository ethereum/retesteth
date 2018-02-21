#
# renames the file if it is different from its destination
include(CMakeParseArguments)
#
macro(replace_if_different SOURCE DST)
	set(extra_macro_args ${ARGN})
	set(options CREATE)
	set(one_value_args)
	set(multi_value_args)
	cmake_parse_arguments(REPLACE_IF_DIFFERENT "${options}" "${one_value_args}" "${multi_value_args}" "${extra_macro_args}")

	if (REPLACE_IF_DIFFERENT_CREATE AND (NOT (EXISTS "${DST}")))
		file(WRITE "${DST}" "")
	endif()

	execute_process(COMMAND ${CMAKE_COMMAND} -E compare_files "${SOURCE}" "${DST}" RESULT_VARIABLE DIFFERENT OUTPUT_QUIET ERROR_QUIET)

	if (DIFFERENT)
		execute_process(COMMAND ${CMAKE_COMMAND} -E rename "${SOURCE}" "${DST}")
	else()
		execute_process(COMMAND ${CMAKE_COMMAND} -E remove "${SOURCE}")
	endif()
endmacro()


# In Windows split repositories build we need to be checking whether or not
# Debug/Release or both versions were built for the config phase to run smoothly
macro(eth_check_library_link L)
	if (${${L}_LIBRARY} AND ${${L}_LIBRARY} EQUAL "${L}_LIBRARY-NOTFOUND")
		unset(${${L}_LIBRARY})
	endif()
	if (${${L}_LIBRARY_DEBUG} AND ${${L}_LIBRARY_DEBUG} EQUAL "${L}_LIBRARY_DEBUG-NOTFOUND")
		unset(${${L}_LIBRARY_DEBUG})
	endif()
	if (${${L}_LIBRARY} AND ${${L}_LIBRARY_DEBUG})
		set(${L}_LIBRARIES optimized ${${L}_LIBRARY} debug ${${L}_LIBRARY_DEBUG})
	elseif (${${L}_LIBRARY})
		set(${L}_LIBRARIES ${${L}_LIBRARY})
	elseif (${${L}_LIBRARY_DEBUG})
		set(${L}_LIBRARIES ${${L}_LIBRARY_DEBUG})
	endif()
endmacro()

