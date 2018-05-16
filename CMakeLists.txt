#-------------------------------------------------
# bolib2 CMake
#-------------------------------------------------
cmake_minimum_required(VERSION 3.10)


if(NOT CMAKE_BUILD_TYPE)
	message(STATUS "No build type selected, default to Debug")
	set(CMAKE_BUILD_TYPE "Debug" CACHE STRING 
                "Choose the type of build, options are: Debug Release RelWithDebInfo MinSizeRel." FORCE)
endif()
option(BOLIB2_EXECUTE_TEST "Execute test suite" ON)
option(BOLIB2_COVERAGE "Check code coverage" ON)
set(BOLIB2_COVERAGE_EXECUTE FALSE)

project(bolib2)
include(CheckIncludeFiles)
include(cmake/macs.cmake)

add_subdirectory(ext)
add_subdirectory(src)

if(BOLIB2_EXECUTE_TEST)
	find_program(GCOV gcov)
	if(NOT GCOV)
		message(FATAL_ERROR "-- gcov not found...")
	endif()
	message("-- gcov found: ${GCOV}")

	add_subdirectory(test)
	add_test(NAME bolib2 COMMAND bolib2_test)
	add_custom_target(check ALL
		COMMAND ${CMAKE_COMMAND} -E echo "-- Prepareing test data..."
		COMMAND ${CMAKE_COMMAND} -E remove_directory "${CMAKE_BINARY_DIR}/testdata"
		COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_SOURCE_DIR}/testdata" "${CMAKE_BINARY_DIR}/testdata"
		COMMAND ${CMAKE_COMMAND} -E echo "-- Executes bolib2_test ..."
		COMMAND test/bolib2_test -c
		COMMAND ${CMAKE_COMMAND} -E echo "-- Removing test data..."
		COMMAND ${CMAKE_COMMAND} -E remove_directory "${CMAKE_BINARY_DIR}/testdata"
		)
	add_dependencies(check bolib2_test)
	add_dependencies(bolib2_exe check)
	if(BOLIB2_COVERAGE_EXECUTE)
		add_custom_target(coverage ALL
			COMMAND rm -rf gcov
			COMMAND mkdir gcov
			COMMAND find src/CMakeFiles/bolib2.dir/ -type f -name "*.gcno" -print0 | xargs -0 env LANG=C gcov
			COMMAND mv *.gcov gcov
			)
		add_dependencies(coverage check)
		add_dependencies(bolib2_exe coverage)
	endif(BOLIB2_COVERAGE_EXECUTE)
endif(BOLIB2_EXECUTE_TEST)



