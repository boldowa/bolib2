/**
 * @file TestRomFileTest.cpp
 */
#include <CppUTest/TestHarness.h>
#include "memory/bmem.h"
#include "file/TestRomFile.h"

TEST_GROUP(TestRomFile)
{
	RomFile* target;
	virtual void setup()
	{
		target = new_TestRomFile("l1024k");
	}

	virtual void teardown()
	{
		delete_TestRomFile(&target);
	}
};


/**
 * Check object create
 */
TEST(TestRomFile, new)
{
	CHECK(NULL != target);

	/* Methods */
	CHECK(NULL!=target->type_get);
	CHECK(NULL!=target->mapmode_get);
	CHECK(NULL!=target->sum_get);
	CHECK(NULL!=target->reload);
	CHECK(NULL!=target->write);
	CHECK(NULL!=target->IsValidSum);
	CHECK(NULL!=target->GetSnesPtr);
	CHECK(NULL!=target->GetPcPtr);
	CHECK(NULL!=target->Pc2SnesAdr);
	CHECK(NULL!=target->Snes2PcAdr);
	CHECK(NULL!=target->RatsSearch);
	CHECK(NULL!=target->RatsClean);
	CHECK(NULL!=target->UseHiRomMapSA1);
	CHECK(NULL!=target->ChecksumUpdate);
	CHECK(NULL!=target->DetectRomType);
	CHECK(NULL!=target->SearchFreeSpace);
	CHECK(NULL!=target->HasHeader);

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
	LONGS_EQUAL(1024*1024, target->size_get(target));
	LONGS_EQUAL(RomType_LoRom, target->type_get(target));
	LONGS_EQUAL(MapMode_20, target->mapmode_get(target));
}

/**
 * Check object delete
 */
TEST(TestRomFile, delete)
{
	delete_TestRomFile(&target);

	/* check delete */
	POINTERS_EQUAL(NULL, target);
}

/**
 * Check open function
 */
TEST(TestRomFile, open)
{
	LONGS_EQUAL(FileOpen_NoError, target->open(target));

	/* check rom info */
	LONGS_EQUAL(MapMode_20, target->mapmode_get(target));
	LONGS_EQUAL(RomType_LoRom, target->type_get(target));
	LONGS_EQUAL(1024*1024, target->size_get(target));

	/* close */
	target->close(target);
}

/**
 * check reload method
 */
TEST(TestRomFile, reload)
{
	uint8* ptr;

	LONGS_EQUAL(FileOpen_NoError, target->open(target));

	/* write data */
	ptr = target->GetSnesPtr(target, 0x8000);
	write16(ptr, 0x1234);
	LONGS_EQUAL(0x1234, read16(ptr));

	/* reload */
	CHECK_FALSE(target->reload(target));

	target->close(target);
}

/**
 * check close method
 */
TEST(TestRomFile, close)
{
	/* Nothing to do here */
}

