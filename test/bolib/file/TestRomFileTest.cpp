/**
 * @file TestRomFileTest.cpp
 */
#include <CppUTest/TestHarness.h>
#include <memory.h>
#include "bolib/memory/bmem.h"
#include "bolib/file/TestRomFile.h"

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
	LONGS_EQUAL(ROMADDRESS_NULL, target->Snes2PcAdr(target, 0x8000));
	LONGS_EQUAL(ROMADDRESS_NULL, target->Pc2SnesAdr(target, 0x0000));
	POINTERS_EQUAL(NULL, target->GetPcPtr(target, 0))
	POINTERS_EQUAL(NULL, target->GetSnesPtr(target, 0))
	LONGS_EQUAL(ROMADDRESS_NULL, target->RatsSearch(target, 0x8000, NULL));
	CHECK_FALSE(target->RatsClean(target, 0x8000));
	CHECK_FALSE(target->HasHeader(target));
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
	CHECK(target->write(target));

	/* reload */
	CHECK_FALSE(target->reload(target));

	target->close(target);

	POINTERS_EQUAL(NULL, target->GetPcPtr(target, 0))
	POINTERS_EQUAL(NULL, target->GetSnesPtr(target, 0))
	LONGS_EQUAL(ROMADDRESS_NULL, target->RatsSearch(target, 0x8000, NULL));
	CHECK_FALSE(target->RatsClean(target, 0x8000));
	CHECK_FALSE(target->HasHeader(target));
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


TEST(TestRomFile, UnknownRom)
{
	target = new_TestRomFile("x1024");
	LONGS_EQUAL(FileOpen_NoError, target->open(target));

	/* check rom info */
	LONGS_EQUAL(MapMode_Unknown, target->mapmode_get(target));
	LONGS_EQUAL(RomType_Unknown, target->type_get(target));
	LONGS_EQUAL(1024*1024, target->size_get(target));

	/* close */
	target->close(target);
}


TEST(TestRomFile, SearchFreeSpace)
{
	SA1AdrInfo sa1inf;
	uint8 i;
	uint8 *p;

	for(i=0; i<4; i++)
	{
		sa1inf.slots[i] = (uint8)(i+4);
		sa1inf.mbits[i] = false;
	}
	target = new_TestRomFile("s6144");
	LONGS_EQUAL(FileOpen_NoError, target->open(target));
	target->SetSA1Info(target, sa1inf);

	/* write dummy data */
	p = target->GetPcPtr(target, 0x80000);
	memset(p, 0xff, 1024*1024*5);
	/* write rats tag */
	memcpy(p,"STAR\x00\x01\xff\xfe",8);

	/* check */
	LONGS_EQUAL(0xd80000, target->SearchFreeSpace(target, 0x200));
	LONGS_EQUAL(ROMADDRESS_NULL, target->SearchFreeSpace(target, 0x8000));
}


TEST(TestRomFile, SearchFreeSpace_HiROM)
{
	uint8 *p;

	/* HiROM */
	target = new_TestRomFile("h2048");
	LONGS_EQUAL(FileOpen_NoError, target->open(target));

	/* write dummy data */
	p = target->GetPcPtr(target, 0x80000);
	memset(p, 0xff, 1024*1024);
	/* write rats tag */
	memcpy(p,"STAR\x00\x01\xff\xfe",8);

	/* check */
	LONGS_EQUAL(0xd80000, target->SearchFreeSpace(target, 0x200));
}


TEST(TestRomFile, SearchFreeSpace_ExHiROM)
{
	uint8 *p;

	/* HiROM */
	target = new_TestRomFile("H6144");
	LONGS_EQUAL(FileOpen_NoError, target->open(target));

	/* write dummy data */
	p = target->GetPcPtr(target, 0x80000);
	memset(p, 0xff, 1024*1024);
	/* write rats tag */
	memcpy(p,"STAR\x00\x01\xff\xfe",8);

	/* write dummy data */
	p = target->GetPcPtr(target, 0x80000);
	memset(p, 0xff, 1024*1024*5);
	/* write rats tag */
	memcpy(p,"STAR\x00\x01\xff\xfe",8);

	/* check */
	LONGS_EQUAL(0x580000, target->SearchFreeSpace(target, 0x200));
}
