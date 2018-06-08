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
	target = new_TestRomFile("l1024k");
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
	target = new_TestRomFile("l1024k");
	delete_TestRomFile(&target);

	/* check delete */
	POINTERS_EQUAL(NULL, target);
}

/**
 * Check open function
 */
TEST(TestRomFile, open)
{
	target = new_TestRomFile("l1024k");
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

	target = new_TestRomFile("l1024k");
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


TEST(TestRomFile, SA1Rom)
{
	target = new_TestRomFile("s1024");
	CHECK(NULL != target);

	/* check initial value */
	LONGS_EQUAL(1024*1024, target->size_get(target));
	LONGS_EQUAL(RomType_LoRom, target->type_get(target));
	LONGS_EQUAL(MapMode_SA1, target->mapmode_get(target));
}


TEST(TestRomFile, SuperFX)
{
	target = new_TestRomFile("g2048");
	CHECK(NULL != target);

	/* check initial value */
	LONGS_EQUAL(1024*2048, target->size_get(target));
	LONGS_EQUAL(RomType_LoRom, target->type_get(target));
	LONGS_EQUAL(MapMode_20, target->mapmode_get(target));
}

TEST(TestRomFile, HiRom)
{
	target = new_TestRomFile("h1024");
	CHECK(NULL != target);

	/* check initial value */
	LONGS_EQUAL(1024*1024, target->size_get(target));
	LONGS_EQUAL(RomType_HiRom, target->type_get(target));
	LONGS_EQUAL(MapMode_21, target->mapmode_get(target));
}


TEST(TestRomFile, ExLoROM)
{
	target = new_TestRomFile("L6144");
	CHECK(NULL != target);

	/* check initial value */
	LONGS_EQUAL(1024*6144, target->size_get(target));
	LONGS_EQUAL(RomType_ExLoRom, target->type_get(target));
	LONGS_EQUAL(MapMode_25, target->mapmode_get(target));
}


TEST(TestRomFile, ExHiROM)
{
	target = new_TestRomFile("H6144");
	CHECK(NULL != target);

	/* check initial value */
	LONGS_EQUAL(1024*6144, target->size_get(target));
	LONGS_EQUAL(RomType_ExHiRom, target->type_get(target));
	LONGS_EQUAL(MapMode_25, target->mapmode_get(target));
}


TEST(TestRomFile, TooSmall)
{
	target = new_TestRomFile("l1");
	POINTERS_EQUAL(NULL, target);
}
