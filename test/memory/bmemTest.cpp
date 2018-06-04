/**
 * @file bmemTest.cpp
 */
#include <CppUTest/TestHarness.h>
#include "memory/bmem.h"

TEST_GROUP(bmem)
{
	uint8 testarr[8];
	virtual void setup()
	{
		testarr[0] = 0x12;
		testarr[1] = 0x34;
		testarr[2] = 0x56;
	}

	virtual void teardown()
	{
	}
};


/**
 * Check read16
 */
TEST(bmem, read16)
{
	LONGS_EQUAL(0x3412, read16(&testarr[0]));
	LONGS_EQUAL(0x5634, read16(&testarr[1]));
}


/**
 * Check read24
 */
TEST(bmem, read24)
{
	LONGS_EQUAL(0x563412, read24(&testarr[0]));
}


/**
 * Check write16
 */
TEST(bmem, write16)
{
	write16(&testarr[0], 0x1122);
	LONGS_EQUAL(0x1122, read16(&testarr[0]));
}


/**
 * Check write24
 */
TEST(bmem, write24)
{
	write24(&testarr[0], 0x334566);
	LONGS_EQUAL(0x334566, read24(&testarr[0]));
}


/**
 * Check bmsearch
 */
TEST(bmem, bmsearch)
{
	/* TODO: Write test code. */
}


/*--- (!)CAUTION(!) Don't edit this comment! ---*/

