/**
 * @file libfileTest.cpp
 */
#include <CppUTest/TestHarness.h>
#include <string.h>
#include "../TestCommon.h"
#include "bolib/file/libfile.h"

TEST_GROUP(libfileTest)
{
	virtual void setup()
	{
	}

	virtual void teardown()
	{
	}
};


/**
 * Check fexists
 */
TEST(libfileTest, fexists)
{
	/* file exists check. */
	CHECK(fexists("./testdata/file/exists.txt"));
	CHECK_FALSE(fexists("./testdata/file/notexists.txt"));

	/* directory */
	CHECK_FALSE(fexists("./testdata"))
}


/**
 * Check dexists
 */
TEST(libfileTest, dexists)
{
	/* directory exists check */
	CHECK(dexists("./testdata"))
	CHECK_FALSE(dexists("./notexists"))

	/* file */
	CHECK_FALSE(dexists("./testdata/file/notexists.txt"));
}


/**
 * Check gettmpdir
 */
TEST(libfileTest, gettmpdir)
{
#if isWindows
	FAIL("?????");
#else
	char* tmpdir;

	/* check */
	tmpdir = gettmpdir();
	CHECK(NULL != tmpdir);
	STRCMP_EQUAL("/tmp/", tmpdir);

	/* clean */
	free(tmpdir);
#endif
}


/**
 * Check abspath
 */
TEST(libfileTest, abspath)
{
	/* TODO: Write test code. */
	char* apath;

	apath = abspath(".");
	CHECK(NULL != apath);
	CHECK(0 != strcmp(".",apath));

	free(apath);
}


/**
 * Check fsize
 */
TEST(libfileTest, fsize)
{
	/* directory */
	LONGS_EQUAL(0,  fsize("./testdata/file"));

	/* not exists */
	LONGS_EQUAL(0,  fsize("./testdata/file/notexists.txt"));

	/* exists */
	LONGS_EQUAL(31, fsize("./testdata/file/exists.txt"));
}


/**
 * Check makedir
 */
TEST(libfileTest, makedir)
{
	/* directory exists check */
	CHECK_FALSE(dexists("./testdata/file/mkdir"));
	CHECK(makedir(OutDir "mkdir"));
	CHECK(dexists(OutDir "mkdir"));
}


/**
 * Check getworkdir
 */
TEST(libfileTest, getworkdir)
{
	/* TODO: Write test code. */
#if isWindows
	FAIL("?????");
#else
	const static char* dirpath = "/tmp/dwork/";
	char* workdir;

	/* check */
	workdir = getworkdir("dwork/");
	CHECK(NULL != workdir);
	STRCMP_EQUAL(dirpath, workdir);
	CHECK(dexists(dirpath));

	/* clean */
	free(workdir);
#endif
}


/*--- (!)CAUTION(!) Don't edit this comment! ---*/

