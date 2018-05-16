/**
 * @file bolib2Test.cpp
 *   This is a test for bolib2 main function.
 */
#include <CppUTest/TestHarness.h>

extern "C" int bolib2(int argc, char** argv);

TEST_GROUP(bolib2)
{
};

TEST(bolib2, case1)
{
	/* TODO: Write main test. */
	char* dummy[] = {(char*)"dummy"};
	LONGS_EQUAL(0, bolib2(1, (char**)dummy));
}

