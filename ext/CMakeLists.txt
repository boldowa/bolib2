#--- Builds CppUTest
option(EXTENSIONS "Uses CppUTestExt" OFF)
option(TESTS "Tests CppUTest library" OFF)

if(BOLIB2_EXECUTE_TEST)
	add_subdirectory(cpputest)
endif(BOLIB2_EXECUTE_TEST)

