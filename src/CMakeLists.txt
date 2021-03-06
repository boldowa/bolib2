#-------------------------------------------------
# bolib2 main source
#-------------------------------------------------

# Compile flags
if(MSVC)
	set(BOLIB2_FLAGS "")
else(MSVC)
	set(BOLIB2_FLAGS "")
	# encode flags
	if(WINDOWS)
		add_flags(BOLIB2_FLAGS "-fexec-charset=cp932")
	endif(WINDOWS)
	# Warning flags
	add_flags(BOLIB2_FLAGS "-W -Wpedantic -Wno-format -Wconversion -Wimplicit-function-declaration")
	# Test flags
	if(BOLIB2_TESTBUILD)
		add_flags(BOLIB2_FLAGS "-include CppUTest/MemoryLeakDetectorNewMacros.h")
		add_flags(BOLIB2_FLAGS "-include CppUTest/MemoryLeakDetectorMallocMacros.h")
	endif(BOLIB2_TESTBUILD)
	if(BOLIB2_EXECUTE_TEST)
		if(BOLIB2_COVERAGE)
			add_flags(BOLIB2_FLAGS "--coverage")
		endif(BOLIB2_COVERAGE)
	endif(BOLIB2_EXECUTE_TEST)
endif(MSVC)

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
	if(BOLIB2_COVERAGE)
		target_link_libraries(bolib2 gcov)
		set(BOLIB2_COVERAGE_EXECUTE TRUE PARENT_SCOPE)
	endif()
endif(NOT MSVC)
set_target_properties(bolib2 PROPERTIES COMPILE_FLAGS ${BOLIB2_FLAGS})

add_executable(bolib2_exe bolib/main.c)
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
		#message(STATUS "install(TARGETS bolib2")
		#message(STATUS "	RUNTIME DESTINATION \"${INSTALL_BIN_DIR}\"")
		#message(STATUS "	ARCHIVE DESTINATION \"${INSTALL_LIB_DIR}\"")
		#message(STATUS "	LIBRARY DESTINATION \"${INSTALL_LIB_DIR}\" )")
		# headers
		install(DIRECTORY "${PROJECT_SOURCE_DIR}/include/${BOLIB_INCDIR_NAME}"
			DESTINATION "${INSTALL_INC_DIR}"
			FILES_MATCHING PATTERN "*.h")
		install(FILES "${PROJECT_BINARY_DIR}/bolib.h" DESTINATION "${INSTALL_INC_DIR}")
		install(FILES
			"${PROJECT_BINARY_DIR}/${BOLIB_INCDIR_NAME}/btypes.h"
			"${PROJECT_BINARY_DIR}/${BOLIB_INCDIR_NAME}/int_type.h"
			DESTINATION "${INSTALL_INC_DIR}/${BOLIB_INCDIR_NAME}")
		# package config
		install(FILES "${PROJECT_BINARY_DIR}/${BOLIB_PC}" DESTINATION "${INSTALL_PKGCONFIG_DIR}")
	endif(NOT BOLIB_SKIP_INSTALL)
endif("Release" STREQUAL ${CMAKE_BUILD_TYPE})

