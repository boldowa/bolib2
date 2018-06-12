/**
 * @file TextFileTest.cpp
 */
#include <CppUTest/TestHarness.h>
#include <stdio.h>
#include "../TestCommon.h"
#include "bolib/file/TextFile.h"

#define TestRoot "testdata/file/"
#define ReadFile "test.txt"
#define WriteFile "wtest.txt"

#define WriteLine1 "12345"
#define WriteLine2 "12 = %d"

TEST_GROUP(TextFile)
{
	TextFile* target;
	virtual void setup()
	{
		target = new_TextFile(TestRoot ReadFile);
	}

	virtual void teardown()
	{
		delete_TextFile(&target);
	}
};


/**
 * Check object create
 */
TEST(TextFile, new)
{
	CHECK(NULL != target);

	/* method */
	CHECK(NULL!=target->open2);
	CHECK(NULL!=target->row_get);
	CHECK(NULL!=target->getline);
	CHECK(NULL!=target->printf);

	/* inherited method */
	CHECK(NULL!=target->path_get);
	CHECK(NULL!=target->dir_get);
	CHECK(NULL!=target->name_get);
	CHECK(NULL!=target->ext_get);
	CHECK(NULL!=target->open);
	CHECK(NULL!=target->close);
	CHECK(NULL!=target->size_get);

	/* protected member */
	CHECK(NULL!=target->pro);

	/* check initial value */
	LONGS_EQUAL(0, target->size_get(target));
	STRCMP_EQUAL(TestRoot ReadFile, target->path_get(target));
	STRCMP_EQUAL(TestRoot, target->dir_get(target));
	STRCMP_EQUAL("test", target->name_get(target));
	STRCMP_EQUAL(".txt", target->ext_get(target));
}

/**
 * Check object delete
 */
TEST(TextFile, delete)
{
	delete_TextFile(&target);

	/* check delete */
	POINTERS_EQUAL(NULL, target);
}


/**
 * Check open method
 */
TEST(TextFile, open)
{
	/* check to success */
	LONGS_EQUAL(FileOpen_NoError,target->open(target));

	/* check size */
	LONGS_EQUAL(302, target->super.size_get(&target->super));

	/* check to fail */
	LONGS_EQUAL(FileOpen_AlreadyOpen, target->open(target));
}

/**
 * Check open2 method
 */
TEST(TextFile, open2)
{
	/* Nothing to do here */
}

/**
 * Check size_get method
 */
TEST(TextFile, size_get)
{
	/* Nothing to do here */
}

/**
 * Check row_get method
 */
TEST(TextFile, row_get)
{
	/* Nothing to do here */
}


/**
 * Check getline method
 */
TEST(TextFile, getline)
{
	const char* line;
	LONGS_EQUAL(FileOpen_NoError,target->open(target));

	/* line 0 */
	LONGS_EQUAL(0, target->row_get(target));

	/* line 1 */
	line = target->getline(target);
	CHECK(NULL != line);
	LONGS_EQUAL(1, target->row_get(target));
	STRCMP_EQUAL("Testing... golo", line);

	/* line 2 */
	line = target->getline(target);
	CHECK(NULL != line);
	LONGS_EQUAL(2, target->row_get(target));
	STRCMP_EQUAL("1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890", line);

	/* line 3 */
	line = target->getline(target);
	CHECK(NULL != line);
	LONGS_EQUAL(3, target->row_get(target));
	STRCMP_EQUAL("xxx", line);

	/* line 4 */
	line = target->getline(target);
	CHECK(NULL != line);
	LONGS_EQUAL(4, target->row_get(target));
	STRCMP_EQUAL("", line);

	/* line end */
	line = target->getline(target);
	LONGS_EQUAL(4, target->row_get(target));
	POINTERS_EQUAL(NULL, line);
}

TEST_GROUP(TextFile2)
{
	/* test target */
	TextFile* target;
	TextFile* reader;

	const char* line1 = "12345\n";
	const char* line2 = "12 = %d\n";

	void setup()
	{
		target = new_TextFile(OutDir WriteFile);
		reader = new_TextFile(OutDir WriteFile);

		::remove(TestRoot WriteFile);
	}

	void teardown()
	{
		delete_TextFile(&target);
		delete_TextFile(&reader);
		::remove(TestRoot WriteFile);
	}
};

/**
 * Check printf method
 */
TEST(TextFile2, printf)
{
	const char* line;

	/* check file not exists */
	LONGS_EQUAL(FileOpen_CantAccess, reader->open(target));

	/* Create new file */
	LONGS_EQUAL(FileOpen_NoError, target->open2(target, "w"));

	/* puts */
	target->printf(target, WriteLine1 "\n");
	target->printf(target, WriteLine2, 12);

	target->close(target);

	/* check file read */
	LONGS_EQUAL(FileOpen_NoError, reader->open(reader));

	line = reader->getline(reader);
	CHECK(NULL != line);
	STRCMP_EQUAL(WriteLine1, line);

	line = reader->getline(reader);
	CHECK(NULL != line);
	STRCMP_EQUAL("12 = 12", line);

	reader->close(reader);
}

