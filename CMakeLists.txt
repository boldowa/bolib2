#-------------------------------------------------
# bolib2 CMake
#-------------------------------------------------
cmake_minimum_required(VERSION 3.6)

include(CheckIncludeFiles)
include(cmake/macs.cmake)

if(NOT CMAKE_BUILD_TYPE)
	message(STATUS "No build type selected, default to Debug")
	set(CMAKE_BUILD_TYPE "Debug" CACHE STRING 
                "Choose the type of build, options are: Debug Release RelWithDebInfo MinSizeRel." FORCE)
endif()
option(BOLIB2_EXECUTE_TEST "Execute test suite" ON)
option(BOLIB2_COVERAGE "Check code coverage" ON)
set(BOLIB2_COVERAGE_EXECUTE FALSE)

if(NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
	set(BOLIB2_EXECUTE_TEST OFF)
endif()

set(BOLIB_VERSION "0.0.1")
set(BOLIB_INCDIR_NAME "bolib")
set(BOLIB_PC "bolib.pc")
set(INSTALL_BIN_DIR "${CMAKE_INSTALL_PREFIX}/bin" CACHE PATH "Installation directory for executables")
set(INSTALL_LIB_DIR "${CMAKE_INSTALL_PREFIX}/lib" CACHE PATH "Installation directory for libraries")
set(INSTALL_INC_DIR "${CMAKE_INSTALL_PREFIX}/include" CACHE PATH "Installation directory for headers")
set(INSTALL_MAN_DIR "${CMAKE_INSTALL_PREFIX}/share/man" CACHE PATH "Installation directory for manual pages")
set(INSTALL_PKGCONFIG_DIR "${CMAKE_INSTALL_PREFIX}/share/pkgconfig" CACHE PATH "Installation directory for pkgconfig (.pc) files")

project(bolib2)

# GLOBAL
if(UNIX)
	set(CMAKE_C_FLAGS_DEBUG   "-O0 -g3")
	set(CMAKE_C_FLAGS_RELEASE "-O2 -DNDEBUG")
endif(UNIX)

# configure
check_include_files(cstdint HAVE_CSTDINT)
check_include_files(stdbool.h HAVE_STDBOOL_H)
check_include_files(stdint.h HAVE_STDINT_H)
check_include_files(inttypes.h HAVE_INTTYPES_H)
check_include_files(sys/types.h.h HAVE_SYS_TYPES_H)
configure_file("${PROJECT_SOURCE_DIR}/include/int_type.h.in" "${PROJECT_BINARY_DIR}/int_type.h")
configure_file("${PROJECT_SOURCE_DIR}/include/btypes.h.in" "${PROJECT_BINARY_DIR}/btypes.h")
configure_file("${PROJECT_SOURCE_DIR}/bolib.h.in" "${PROJECT_BINARY_DIR}/bolib.h" @ONLY)
configure_file("${PROJECT_SOURCE_DIR}/bolib.pc.in" "${PROJECT_BINARY_DIR}/${BOLIB_PC}" @ONLY)

add_subdirectory(ext)
add_subdirectory(src)

if(BOLIB2_EXECUTE_TEST)
	find_program(GCOV gcov)
	if(NOT GCOV)
		message(FATAL_ERROR "-- gcov not found...")
	endif()
	message("-- gcov found: ${GCOV}")

	add_subdirectory(test)
	enable_testing()
	add_test(NAME bolib2 COMMAND bolib2_test)
	add_custom_target(check ALL
		COMMAND ${CMAKE_COMMAND} -E echo "+++++ Test starts... +++++"
		COMMAND ${CMAKE_COMMAND} -E echo "-- Prepareing test data..."
		#COMMAND ${CMAKE_COMMAND} -E remove_directory "${CMAKE_BINARY_DIR}/testdata"
		COMMAND ${CMAKE_COMMAND} -E echo "   copy_directory \"${PROJECT_SOURCE_DIR}/testdata\" \"${CMAKE_BINARY_DIR}/testdata\""
		COMMAND ${CMAKE_COMMAND} -E copy_directory "${PROJECT_SOURCE_DIR}/testdata" "${CMAKE_BINARY_DIR}/testdata"
		COMMAND ${CMAKE_COMMAND} -E echo "   remove_directory \"${CMAKE_BINARY_DIR}/testdata/@out\""
		COMMAND ${CMAKE_COMMAND} -E remove_directory "${CMAKE_BINARY_DIR}/testdata/@out"
		COMMAND ${CMAKE_COMMAND} -E echo "   make_directory \"${CMAKE_BINARY_DIR}/testdata/@out\""
		COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_BINARY_DIR}/testdata/@out"
		COMMAND ${CMAKE_COMMAND} -E echo "-- Executes bolib2_test ..."
		COMMAND $<TARGET_FILE:bolib2_test> -c
		COMMAND ${CMAKE_COMMAND} -E echo "-- Removing test data..."
		COMMAND ${CMAKE_COMMAND} -E echo "   remove_directory \"${CMAKE_BINARY_DIR}/testdata/@out\""
		COMMAND ${CMAKE_COMMAND} -E remove_directory "${CMAKE_BINARY_DIR}/testdata/@out"
		COMMAND ${CMAKE_COMMAND} -E echo "+++++ done. +++++"
		)
	add_dependencies(check bolib2_test)
	add_dependencies(bolib2_exe check)

	find_program(GDB gdb)
	if(NOT GDB)
		message(WARNING "gdb isn't found.")
	else(NOT GDB)
		# debug
		add_custom_target(debug
			COMMAND ${CMAKE_COMMAND} -E echo "-- Prepareing test data..."
			#COMMAND ${CMAKE_COMMAND} -E remove_directory "${CMAKE_BINARY_DIR}/testdata"
			COMMAND ${CMAKE_COMMAND} -E echo "   copy_directory \"${PROJECT_SOURCE_DIR}/testdata\" \"${CMAKE_BINARY_DIR}/testdata\""
			COMMAND ${CMAKE_COMMAND} -E copy_directory "${PROJECT_SOURCE_DIR}/testdata" "${CMAKE_BINARY_DIR}/testdata"
			COMMAND ${CMAKE_COMMAND} -E echo "   remove_directory \"${CMAKE_BINARY_DIR}/testdata/@out\""
			COMMAND ${CMAKE_COMMAND} -E remove_directory "${CMAKE_BINARY_DIR}/testdata/@out"
			COMMAND ${CMAKE_COMMAND} -E echo "   make_directory \"${CMAKE_BINARY_DIR}/testdata/@out\""
			COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_BINARY_DIR}/testdata/@out"
			COMMAND ${CMAKE_COMMAND} -E echo "-- Executes gdb..."
			COMMAND gdb $<TARGET_FILE:bolib2_test>
			)
		add_dependencies(debug bolib2_test)
	endif(NOT GDB)

	# coverage
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


set(CMAKE_BUILD_FLAGS "${CMAKE_C_FLAGS}")
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
	add_flags(CMAKE_BUILD_FLAGS ${CMAKE_C_FLAGS_DEBUG})
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
	add_flags(CMAKE_BUILD_FLAGS ${CMAKE_C_FLAGS_RELEASE})
endif()

message("-- CMake compile flags(${CMAKE_BUILD_TYPE}): ${CMAKE_C_COMPILER} ${CMAKE_BUILD_FLAGS}")

