/**
 * @file StrTest.cpp
 */
#include <CppUTest/TestHarness.h>
#include <stdlib.h>
#include <string.h>
#include "data/Str.h"

TEST_GROUP(StrTest)
{
	const char* ST = "('e' *)< Hello, my son, and my daughter.";
	const char* ST_REP = "('e' *)< Hello, your son, and your daughter.";
	const char* ST_REP2 = "('x' *)< Hxllo, your son, and your daughtxr.";

	char* tgt;

	virtual void setup()
	{
		tgt = (char*)calloc(strlen(ST)+1, sizeof(char));
		strcpy_s(tgt, strlen(ST), ST);
	}

	virtual void teardown()
	{
		free(tgt);
	}
};

TEST(StrTest, Str_copy)
{
	char* copy;

	copy = Str_copy(ST);

	CHECK(NULL != copy);
	CHECK(ST != copy);

	LONGS_EQUAL(strlen(ST), strlen(copy));
	STRCMP_EQUAL(ST, copy);

	POINTERS_EQUAL(NULL, Str_copy(NULL));

	free(copy);
}


/*--- (!)CAUTION(!) Don't edit this comment! ---*/

