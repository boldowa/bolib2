#-------------------------------------------------
# BOLIB2 test
#-------------------------------------------------

set(CPPUTEST_LIB CppUTest)

# Compile flags
set(BOLIB2_TEST_FLAGS "-Wall")

if(EXTENSIONS)
	list(APPEND CPPUTEST_LIB CppUTestExt)
endif(EXTENSIONS)

file(GLOB_RECURSE TESTSOURCES
	"."  "*.cpp"
)

add_executable(bolib2_test ${TESTSOURCES})
target_link_libraries(bolib2_test bolib2 ${CPPUTEST_LIB})
target_include_directories(bolib2_test PRIVATE
	"${PROJECT_BINARY_DIR}"
	"${CMAKE_CURRENT_SOURCE_DIR}/../include"
	"${CMAKE_CURRENT_SOURCE_DIR}/../ext/cpputest/include"
)

if(NOT MSVC)
	if(CMAKE_BUILD_TYPE STREQUAL "Debug")
		if(BOLIB2_EXECUTE_TEST)
			add_flags(BOLIB2_TEST_FLAGS "-include CppUTest/MemoryLeakDetectorNewMacros.h")
			add_flags(BOLIB2_TEST_FLAGS "-include CppUTest/MemoryLeakDetectorMallocMacros.h")
		endif()
	endif()
endif(NOT MSVC)
set_target_properties(bolib2_test PROPERTIES COMPILE_FLAGS ${BOLIB2_TEST_FLAGS})


