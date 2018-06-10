#-------------------------------------------------
# bolib2 main source
#-------------------------------------------------

# Compile flags
set(BOLIB2_FLAGS "-Wall -Wpedantic -Wno-format -Wconversion -Wimplicit-function-declaration -ansi")

# Compile All sources
file(GLOB_RECURSE PRJCSOURCES
	"."  "*.c"
)
file(GLOB_RECURSE PRJCPPSOURCES
	"."  "*.cpp"
)
# exclude main.c
list(FILTER PRJCSOURCES EXCLUDE REGEX ".*main.c$")
list(FILTER PRJCPPSOURCES EXCLUDE REGEX ".*main.cpp$")

add_library(bolib2 ${PRJCSOURCES} ${PRJCPPSOURCES})
target_include_directories(bolib2 PRIVATE
	"${PROJECT_BINARY_DIR}"
	"${CMAKE_CURRENT_SOURCE_DIR}/../include"
	"${CMAKE_CURRENT_SOURCE_DIR}/../ext/cpputest/include"
)

if(NOT MSVC)
	if(CMAKE_BUILD_TYPE STREQUAL "Debug")
		if(BOLIB2_EXECUTE_TEST)
			add_flags(BOLIB2_FLAGS "-include CppUTest/MemoryLeakDetectorNewMacros.h")
			add_flags(BOLIB2_FLAGS "-include CppUTest/MemoryLeakDetectorMallocMacros.h")
		endif()

		if(BOLIB2_COVERAGE)
			add_flags(BOLIB2_FLAGS "--coverage")
			target_link_libraries(bolib2 gcov)
			set(BOLIB2_COVERAGE_EXECUTE TRUE PARENT_SCOPE)
		endif()
	endif()
endif(NOT MSVC)
set_target_properties(bolib2 PROPERTIES COMPILE_FLAGS ${BOLIB2_FLAGS})

add_executable(bolib2_exe main.c)
target_link_libraries(bolib2_exe bolib2)
set_target_properties(bolib2_exe PROPERTIES OUTPUT_NAME bolib2)


# Install
if("Release" STREQUAL ${CMAKE_BUILD_TYPE})
	if(NOT BOLIB_SKIP_INSTALL)
		# binary
		install(TARGETS bolib2
			RUNTIME DESTINATION "${INSTALL_BIN_DIR}"
			ARCHIVE DESTINATION "${INSTALL_LIB_DIR}"
			LIBRARY DESTINATION "${INSTALL_LIB_DIR}" )
		# headers
		install(DIRECTORY "${PROJECT_SOURCE_DIR/include}"
			DESTINATION "${INSTALL_INC_DIR}/${BOLIB_INCDIR_NAME}"
			FILES_MATCHING PATTERN "*.h")
		install(FILES "${PROJECT_BINARY_DIR}/bolib.h" DESTINATION "${INSTALL_INC_DIR}")
		install(FILES
			"${PROJECT_BINARY_DIR}/btypes.h"
			"${PROJECT_BINARY_DIR}/int_type.h"
			DESTINATION "${INSTALL_INC_DIR}/${BOLIB_INCDIR_NAME}")
		# package config
		install(FILES "${PROJECT_BINARY_DIR}/${BOLIB_PC}" DESTINATION "${INSTALL_PKGCONFIG_DIR}")
	endif(NOT BOLIB_SKIP_INSTALL)
endif("Release" STREQUAL ${CMAKE_BUILD_TYPE})

