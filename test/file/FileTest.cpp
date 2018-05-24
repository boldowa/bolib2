/**
 * @file FileTest.cpp
 */
#include <CppUTest/TestHarness.h>
#include "file/TextFile.h"
#include "file/File.h"

#define Dir  "testdata/file/"
#define Name "exists"
#define Ext  ".txt"
#define Path Dir Name Ext


TEST_GROUP(File)
{
	File* target;
	virtual void setup()
	{
		target = (File*)new_TextFile(Path);
	}

	virtual void teardown()
	{
		delete_TextFile((TextFile**)&target);
	}
};


/**
 * Check object create
 */
TEST(File, new)
{
	CHECK(NULL != target);

	/* Methods */
	CHECK(NULL!=target->path_get)
	CHECK(NULL!=target->dir_get)
	CHECK(NULL!=target->name_get)
	CHECK(NULL!=target->ext_get)
	CHECK(NULL!=target->open)
	CHECK(NULL!=target->close)
	CHECK(NULL!=target->size_get)
	/* object */
	CHECK(NULL!=target->pro)

	/* check initial value */
	LONGS_EQUAL(0, target->size_get(target));
	STRCMP_EQUAL(Path, target->path_get(target));
	STRCMP_EQUAL(Dir, target->dir_get(target));
	STRCMP_EQUAL(Name, target->name_get(target));
	STRCMP_EQUAL(Ext, target->ext_get(target));
}

/**
 * Check object delete
 */
TEST(File, delete)
{
	/* method isn't implemented */
}


/**
 * Check open method
 */
TEST(File, open)
{
	/* check that fails. */
	LONGS_EQUAL(FileOpen_NoMode, target->open(target));
}


/**
 * Check close method
 */
TEST(File, close)
{
	/* check execute, and no error occures. */
	target->close(target);
}


/**
 * Check size_get method
 */
TEST(File, size_get)
{
	/* Nothing to do */
}
