/**
 * @file MmlFileTest.cpp
 */
#include <CppUTest/TestHarness.h>
#include "file/MmlFile.h"

#define TestRoot "testdata/file/"
#define ReadFile "test.txt"

TEST_GROUP(MmlFile)
{
	MmlFile* target;
	virtual void setup()
	{
		target = new_MmlFile(TestRoot ReadFile);
	}

	virtual void teardown()
	{
		delete_MmlFile(&target);
	}
};


/**
 * Check object create
 */
TEST(MmlFile, new)
{
	CHECK(NULL != target);

	/* Methods */
	/* TODO */

	/* Inherited methods */
	CHECK(NULL!=target->open);
	CHECK(NULL!=target->open2);
	CHECK(NULL!=target->close);
	CHECK(NULL!=target->row_get);
	CHECK(NULL!=target->getline);
	CHECK(NULL!=target->printf);
	CHECK(NULL!=target->path_get);
	CHECK(NULL!=target->dir_get);
	CHECK(NULL!=target->name_get);
	CHECK(NULL!=target->ext_get);
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
TEST(MmlFile, delete)
{
	delete_MmlFile(&target);

	/* check delete */
	POINTERS_EQUAL(NULL, target);
}

