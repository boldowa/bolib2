/**
 * @file StrTest.cpp
 */
#include <CppUTest/TestHarness.h>
#include <stdlib.h>
#include <string.h>
#include "data/Str.h"

TEST_GROUP(Str)
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

TEST(Str, Str_copy)
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


/**
 * Check Str_concat
 */
TEST(Str, Str_concat)
{
	/* Valid value check */
	char* x = Str_concat("aaa", "bbb");
	STRCMP_EQUAL("aaabbb", x);

	/* Invalid value check */
	char* y = Str_concat(NULL, "bbb");
	POINTERS_EQUAL(NULL, y);
	y = Str_concat("aaa", NULL);
	POINTERS_EQUAL(NULL, y);

	free(x);
}


/*--- (!)CAUTION(!) Don't edit this comment! ---*/

