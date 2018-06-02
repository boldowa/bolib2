#-------------------------------------------------
# bolib2 main source
#-------------------------------------------------

# Compile flags
set(BOLIB2_FLAGS "-Wall -Wpedantic -Wno-format -Wconversion -ansi")

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

# configure
check_include_files(cstdint HAVE_CSTDINT)
check_include_files(stdbool.h HAVE_STDBOOL_H)
check_include_files(stdint.h HAVE_STDINT_H)
check_include_files(inttypes.h HAVE_INTTYPES_H)
check_include_files(sys/types.h.h HAVE_SYS_TYPES_H)
configure_file("${PROJECT_SOURCE_DIR}/include/int_type.h.in" "${PROJECT_BINARY_DIR}/int_type.h")
configure_file("${PROJECT_SOURCE_DIR}/include/btypes.h.in" "${PROJECT_BINARY_DIR}/btypes.h")

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

