/**
 * @file RomFileTest_Address.cpp
 *
 *       This test file is tests for address conversion only.
 */
#include <CppUTest/TestHarness.h>
#include "file/TestRomFile.h"


TEST_GROUP(RomFile_Address)
{
	TestRomFile* target;
	virtual void setup()
	{
	}

	virtual void teardown()
	{
		delete_TestRomFile(&target);
	}
};


/**
 * Check LoROM Address conversion
 */
TEST(RomFile_Address, LoROM)
{
	/* create check object */
	target = new_TestRomFile("l1024");
	LONGS_EQUAL(FileOpen_NoError, target->open(target));
	LONGS_EQUAL(RomType_LoRom, target->type_get(target));
	LONGS_EQUAL(MapMode_20, target->mapmode_get(target));

	/*--- Snes2Pc ---*/
	LONGS_EQUAL(0x000000, target->Snes2PcAdr(target, 0x008000));		/**< min */
	LONGS_EQUAL(0x000000, target->Snes2PcAdr(target, 0x808000));		/**< min(High-Speed map) */
	LONGS_EQUAL(0x0fffff, target->Snes2PcAdr(target, 0x1fffff));		/**< max */
	LONGS_EQUAL(0x0fffff, target->Snes2PcAdr(target, 0x9fffff));		/**< max(High-Speed map) */
	LONGS_EQUAL(ROMADDRESS_NULL, target->Snes2PcAdr(target, 0x208000));	/**< overflow */
	LONGS_EQUAL(ROMADDRESS_NULL, target->Snes2PcAdr(target, 0xc00000));	/**< HiRom/SA1 map */
	LONGS_EQUAL(ROMADDRESS_NULL, target->Snes2PcAdr(target, 0x400000));	/**< HiRom/GSU map */
	LONGS_EQUAL(ROMADDRESS_NULL, target->Snes2PcAdr(target, 0x7e8000));	/**< RAM */
	LONGS_EQUAL(ROMADDRESS_NULL, target->Snes2PcAdr(target, 0x000000));	/**< RAM(Mirror) */
	LONGS_EQUAL(ROMADDRESS_NULL, target->Snes2PcAdr(target, 0x007fff));	/**< EXT(Mirror) */

	/*--- Pc2Snes ---*/
	LONGS_EQUAL(0x808000, target->Pc2SnesAdr(target, 0x000000));		/**< min(High-Speed map) */
	LONGS_EQUAL(0x9fffff, target->Pc2SnesAdr(target, 0x0fffff));		/**< max(High-Speed map) */
	LONGS_EQUAL(ROMADDRESS_NULL, target->Pc2SnesAdr(target, 0x100000));	/**< overflow */
}


TEST(RomFile_Address, SuperFX)
{
	/* create check object */
	target = new_TestRomFile("g2048");
	LONGS_EQUAL(FileOpen_NoError, target->open(target));
	LONGS_EQUAL(RomType_LoRom, target->type_get(target));
	LONGS_EQUAL(MapMode_20, target->mapmode_get(target));

	/*--- Snes2Pc ---*/
	LONGS_EQUAL(0x000000, target->Snes2PcAdr(target, 0x008000));		/**< min */
	LONGS_EQUAL(0x1fffff, target->Snes2PcAdr(target, 0x3fffff));		/**< max */
	LONGS_EQUAL(0x000000, target->Snes2PcAdr(target, 0x400000));		/**< min HiRom/GSU map */
	LONGS_EQUAL(0x1fffff, target->Snes2PcAdr(target, 0x5fffff));		/**< max HiRom/GSU map */
	LONGS_EQUAL(ROMADDRESS_NULL, target->Snes2PcAdr(target, 0x808000));	/**< min(High-Speed map) */
	LONGS_EQUAL(ROMADDRESS_NULL, target->Snes2PcAdr(target, 0xbfffff));	/**< max(High-Speed map) */
	LONGS_EQUAL(ROMADDRESS_NULL, target->Snes2PcAdr(target, 0xc00000));	/**< HiRom/SA1 map */
	LONGS_EQUAL(ROMADDRESS_NULL, target->Snes2PcAdr(target, 0x7e8000));	/**< RAM */
	LONGS_EQUAL(ROMADDRESS_NULL, target->Snes2PcAdr(target, 0x000000));	/**< RAM(Mirror) */
	LONGS_EQUAL(ROMADDRESS_NULL, target->Snes2PcAdr(target, 0x007fff));	/**< EXT(Mirror) */

	/*--- Pc2Snes ---*/
	LONGS_EQUAL(0x008000, target->Pc2SnesAdr(target, 0x000000));		/**< min(High-Speed map) */
	LONGS_EQUAL(0x3fffff, target->Pc2SnesAdr(target, 0x1fffff));		/**< max(High-Speed map) */
	LONGS_EQUAL(ROMADDRESS_NULL, target->Pc2SnesAdr(target, 0x200000));	/**< overflow */
}


/**
 * Check HiROM Address conversion
 */
TEST(RomFile_Address, HiROM)
{
	/* create check object */
	target = new_TestRomFile("h1024");
	LONGS_EQUAL(FileOpen_NoError, target->open(target));
	LONGS_EQUAL(RomType_HiRom, target->type_get(target));
	LONGS_EQUAL(MapMode_21, target->mapmode_get(target));

	/*--- Snes2Pc ---*/
	LONGS_EQUAL(0x000000, target->Snes2PcAdr(target, 0xc00000));		/**< min */
	LONGS_EQUAL(0x000000, target->Snes2PcAdr(target, 0x400000));		/**< min(High-Speed map) */
	LONGS_EQUAL(0x0fffff, target->Snes2PcAdr(target, 0xcfffff));		/**< max */
	LONGS_EQUAL(0x0fffff, target->Snes2PcAdr(target, 0x4fffff));		/**< max(High-Speed map) */
	LONGS_EQUAL(0x008000, target->Snes2PcAdr(target, 0x008000));		/**< LoRom map */
	LONGS_EQUAL(0x008000, target->Snes2PcAdr(target, 0x808000));		/**< LoRom map(High-speed map) */
	LONGS_EQUAL(0x0fffff, target->Snes2PcAdr(target, 0x0fffff));		/**< LoROM map max */
	LONGS_EQUAL(0x0fffff, target->Snes2PcAdr(target, 0x8fffff));		/**< LoROM map max(High-Speed map) */
	LONGS_EQUAL(ROMADDRESS_NULL, target->Snes2PcAdr(target, 0xd00000));	/**< overflow */
	LONGS_EQUAL(ROMADDRESS_NULL, target->Snes2PcAdr(target, 0x7e8000));	/**< RAM */
	LONGS_EQUAL(ROMADDRESS_NULL, target->Snes2PcAdr(target, 0x000000));	/**< RAM(Mirror) */
	LONGS_EQUAL(ROMADDRESS_NULL, target->Snes2PcAdr(target, 0x007fff));	/**< EXT(Mirror) */

	/*--- Pc2Snes ---*/
	LONGS_EQUAL(0xc00000, target->Pc2SnesAdr(target, 0x000000));		/**< min(High-Speed map) */
	LONGS_EQUAL(0xcfffff, target->Pc2SnesAdr(target, 0x0fffff));		/**< max(High-Speed map) */
	LONGS_EQUAL(ROMADDRESS_NULL, target->Pc2SnesAdr(target, 0x100000));	/**< overflow */
}


/**
 * Check SA1ROM Snes2PC Address conversion
 */
TEST(RomFile_Address, SA1ROM_Snes2Pc)
{
	SA1AdrInfo sa1inf;

	/* create check object */
	target = new_TestRomFile("s4096");
	LONGS_EQUAL(FileOpen_NoError, target->open(target));
	LONGS_EQUAL(RomType_LoRom, target->type_get(target));
	LONGS_EQUAL(MapMode_SA1, target->mapmode_get(target));

	/* Set SA1 address conversion mode */
	sa1inf.useHiRomMap	= false;	/**< Use LoROM Address conversion mode */
	sa1inf.slots[0]		= 0x00;		/**< Slot[0] : PC $00:0000 - $0f:ffff */
	sa1inf.slots[1]		= 0x10;		/**< Slot[1] : PC $10:0000 - $1f:ffff */
	sa1inf.slots[2]		= 0x20;		/**< Slot[2] : PC $20:0000 - $2f:ffff */
	sa1inf.slots[3]		= 0x30;		/**< Slot[3] : PC $30:0000 - $3f:ffff */
	target->SetSA1Info(target, sa1inf);

	/*--- Snes2Pc LoROM-like ---*/
	LONGS_EQUAL(0x000000, target->Snes2PcAdr(target, 0x008000));		/**< Slot[0] min */
	LONGS_EQUAL(0x0fffff, target->Snes2PcAdr(target, 0x1fffff));		/**< Slot[0] max */
	LONGS_EQUAL(0x100000, target->Snes2PcAdr(target, 0x208000));		/**< Slot[1] min */
	LONGS_EQUAL(0x1fffff, target->Snes2PcAdr(target, 0x3fffff));		/**< Slot[1] max */
	LONGS_EQUAL(0x200000, target->Snes2PcAdr(target, 0x808000));		/**< Slot[2] min */
	LONGS_EQUAL(0x2fffff, target->Snes2PcAdr(target, 0x9fffff));		/**< Slot[2] max */
	LONGS_EQUAL(0x300000, target->Snes2PcAdr(target, 0xa08000));		/**< Slot[3] min */
	LONGS_EQUAL(0x3fffff, target->Snes2PcAdr(target, 0xbfffff));		/**< Slot[3] max */

	/*--- Snes2Pc HiROM-like ---*/
	LONGS_EQUAL(0x000000, target->Snes2PcAdr(target, 0xc00000));		/**< Slot[0] min */
	LONGS_EQUAL(0x0fffff, target->Snes2PcAdr(target, 0xcfffff));		/**< Slot[0] max */
	LONGS_EQUAL(0x100000, target->Snes2PcAdr(target, 0xd00000));		/**< Slot[1] min */
	LONGS_EQUAL(0x1fffff, target->Snes2PcAdr(target, 0xdfffff));		/**< Slot[1] max */
	LONGS_EQUAL(0x200000, target->Snes2PcAdr(target, 0xe00000));		/**< Slot[2] min */
	LONGS_EQUAL(0x2fffff, target->Snes2PcAdr(target, 0xefffff));		/**< Slot[2] max */
	LONGS_EQUAL(0x300000, target->Snes2PcAdr(target, 0xf00000));		/**< Slot[3] min */
	LONGS_EQUAL(0x3fffff, target->Snes2PcAdr(target, 0xffffff));		/**< Slot[3] max */

	/*--- Invalid SNES Address ---*/
	LONGS_EQUAL(ROMADDRESS_NULL, target->Snes2PcAdr(target, 0x408000));	/**< LoRom map */
	LONGS_EQUAL(ROMADDRESS_NULL, target->Snes2PcAdr(target, 0x400000));	/**< HiRom/GSU map */
	LONGS_EQUAL(ROMADDRESS_NULL, target->Snes2PcAdr(target, 0x608000));	/**< LoROM/HiRom/GSU map */
	LONGS_EQUAL(ROMADDRESS_NULL, target->Snes2PcAdr(target, 0x7e8000));	/**< RAM */
	LONGS_EQUAL(ROMADDRESS_NULL, target->Snes2PcAdr(target, 0x000000));	/**< RAM(Mirror) */
	LONGS_EQUAL(ROMADDRESS_NULL, target->Snes2PcAdr(target, 0x007fff));	/**< EXT(Mirror) */

	/*************** Chagne slots ***************/

	/* Set SA1 address conversion mode */
	sa1inf.useHiRomMap	= true; 	/**< Use HiROM Address conversion mode */
	sa1inf.slots[0]		= 0x30;		/**< Slot[0] : PC $30:0000 - $3f:ffff */
	sa1inf.slots[1]		= 0x20;		/**< Slot[1] : PC $20:0000 - $2f:ffff */
	sa1inf.slots[2]		= 0x10;		/**< Slot[2] : PC $10:0000 - $1f:ffff */
	sa1inf.slots[3]		= 0x00;		/**< Slot[3] : PC $00:0000 - $0f:ffff */
	target->SetSA1Info(target, sa1inf);

	/*--- Snes2Pc LoROM-like ---*/
	LONGS_EQUAL(0x300000, target->Snes2PcAdr(target, 0x008000));		/**< Slot[0] min */
	LONGS_EQUAL(0x3fffff, target->Snes2PcAdr(target, 0x1fffff));		/**< Slot[0] max */
	LONGS_EQUAL(0x200000, target->Snes2PcAdr(target, 0x208000));		/**< Slot[1] min */
	LONGS_EQUAL(0x2fffff, target->Snes2PcAdr(target, 0x3fffff));		/**< Slot[1] max */
	LONGS_EQUAL(0x100000, target->Snes2PcAdr(target, 0x808000));		/**< Slot[2] min */
	LONGS_EQUAL(0x1fffff, target->Snes2PcAdr(target, 0x9fffff));		/**< Slot[2] max */
	LONGS_EQUAL(0x000000, target->Snes2PcAdr(target, 0xa08000));		/**< Slot[3] min */
	LONGS_EQUAL(0x0fffff, target->Snes2PcAdr(target, 0xbfffff));		/**< Slot[3] max */

	/*--- Snes2Pc HiROM-like ---*/
	LONGS_EQUAL(0x300000, target->Snes2PcAdr(target, 0xc00000));		/**< Slot[0] min */
	LONGS_EQUAL(0x3fffff, target->Snes2PcAdr(target, 0xcfffff));		/**< Slot[0] max */
	LONGS_EQUAL(0x200000, target->Snes2PcAdr(target, 0xd00000));		/**< Slot[1] min */
	LONGS_EQUAL(0x2fffff, target->Snes2PcAdr(target, 0xdfffff));		/**< Slot[1] max */
	LONGS_EQUAL(0x100000, target->Snes2PcAdr(target, 0xe00000));		/**< Slot[2] min */
	LONGS_EQUAL(0x1fffff, target->Snes2PcAdr(target, 0xefffff));		/**< Slot[2] max */
	LONGS_EQUAL(0x000000, target->Snes2PcAdr(target, 0xf00000));		/**< Slot[3] min */
	LONGS_EQUAL(0x0fffff, target->Snes2PcAdr(target, 0xffffff));		/**< Slot[3] max */

	/*--- Invalid SNES Address ---*/
	LONGS_EQUAL(ROMADDRESS_NULL, target->Snes2PcAdr(target, 0x408000));	/**< LoRom map */
	LONGS_EQUAL(ROMADDRESS_NULL, target->Snes2PcAdr(target, 0x400000));	/**< HiRom/GSU map */
	LONGS_EQUAL(ROMADDRESS_NULL, target->Snes2PcAdr(target, 0x608000));	/**< LoROM/HiRom/GSU map */
	LONGS_EQUAL(ROMADDRESS_NULL, target->Snes2PcAdr(target, 0x7e8000));	/**< RAM */
	LONGS_EQUAL(ROMADDRESS_NULL, target->Snes2PcAdr(target, 0x000000));	/**< RAM(Mirror) */
	LONGS_EQUAL(ROMADDRESS_NULL, target->Snes2PcAdr(target, 0x007fff));	/**< EXT(Mirror) */

	/*************** Chagne slots ***************/

	/* Set SA1 address conversion mode */
	sa1inf.useHiRomMap	= false;	/**< Use LoROM Address conversion mode */
	sa1inf.slots[0]		= 0x38;		/**< Slot[0] : PC $38:0000 - $47:ffff */
	target->SetSA1Info(target, sa1inf);

	/*--- Snes2Pc LoROM-like ---*/
	LONGS_EQUAL(0x380000, target->Snes2PcAdr(target, 0x008000));		/**< Slot[0] min */
	LONGS_EQUAL(0x3fffff, target->Snes2PcAdr(target, 0x0fffff));		/**< Slot[0] max-size */

	/*--- Snes2Pc HiROM-like ---*/
	LONGS_EQUAL(0x380000, target->Snes2PcAdr(target, 0xc00000));		/**< Slot[0] min */
	LONGS_EQUAL(0x3fffff, target->Snes2PcAdr(target, 0xc7ffff));		/**< Slot[0] max-size */

	/*--- Invalid ---*/
	LONGS_EQUAL(ROMADDRESS_NULL, target->Snes2PcAdr(target, 0x108000));	/**< LoRom overflow */
	LONGS_EQUAL(ROMADDRESS_NULL, target->Snes2PcAdr(target, 0xc80000));	/**< HiRom overflow */
}


/**
 * Check SA1ROM PC2Snes Address conversion
 */
TEST(RomFile_Address, SA1ROM_Pc2Snes)
{
	SA1AdrInfo sa1inf;

	/* create check object */
	target = new_TestRomFile("s4096");
	LONGS_EQUAL(FileOpen_NoError, target->open(target));
	LONGS_EQUAL(RomType_LoRom, target->type_get(target));
	LONGS_EQUAL(MapMode_SA1, target->mapmode_get(target));

	/* Set SA1 address conversion mode */
	sa1inf.slots[0]		= 0x00;		/**< Slot[0] : PC $00:0000 - $0f:ffff */
	sa1inf.slots[1]		= 0x10;		/**< Slot[1] : PC $10:0000 - $1f:ffff */
	sa1inf.slots[2]		= 0x20;		/**< Slot[2] : PC $20:0000 - $2f:ffff */
	sa1inf.slots[3]		= 0x30;		/**< Slot[3] : PC $30:0000 - $3f:ffff */

	/*************** LoROM Address mode ***************/
	sa1inf.useHiRomMap	= false;	/**< Use LoROM Address conversion mode */
	target->SetSA1Info(target, sa1inf);
	LONGS_EQUAL(0x008000, target->Pc2SnesAdr(target, 0x000000));		/**< Slot[0] min */
	LONGS_EQUAL(0x1fffff, target->Pc2SnesAdr(target, 0x0fffff));		/**< Slot[0] max */
	LONGS_EQUAL(0x208000, target->Pc2SnesAdr(target, 0x100000));		/**< Slot[1] min */
	LONGS_EQUAL(0x3fffff, target->Pc2SnesAdr(target, 0x1fffff));		/**< Slot[1] max */
	LONGS_EQUAL(0x808000, target->Pc2SnesAdr(target, 0x200000));		/**< Slot[2] min */
	LONGS_EQUAL(0x9fffff, target->Pc2SnesAdr(target, 0x2fffff));		/**< Slot[2] max */
	LONGS_EQUAL(0xa08000, target->Pc2SnesAdr(target, 0x300000));		/**< Slot[3] min */
	LONGS_EQUAL(0xbfffff, target->Pc2SnesAdr(target, 0x3fffff));		/**< Slot[3] max */
	LONGS_EQUAL(ROMADDRESS_NULL, target->Pc2SnesAdr(target, 0x400000));	/**< overflow */

	/*************** HiROM Address mode ***************/
	sa1inf.useHiRomMap	= true; 	/**< Use HiROM Address conversion mode */
	target->SetSA1Info(target, sa1inf);
	LONGS_EQUAL(0xc00000, target->Pc2SnesAdr(target, 0x000000));		/**< Slot[0] min */
	LONGS_EQUAL(0xcfffff, target->Pc2SnesAdr(target, 0x0fffff));		/**< Slot[0] max */
	LONGS_EQUAL(0xd00000, target->Pc2SnesAdr(target, 0x100000));		/**< Slot[1] min */
	LONGS_EQUAL(0xdfffff, target->Pc2SnesAdr(target, 0x1fffff));		/**< Slot[1] max */
	LONGS_EQUAL(0xe00000, target->Pc2SnesAdr(target, 0x200000));		/**< Slot[2] min */
	LONGS_EQUAL(0xefffff, target->Pc2SnesAdr(target, 0x2fffff));		/**< Slot[2] max */
	LONGS_EQUAL(0xf00000, target->Pc2SnesAdr(target, 0x300000));		/**< Slot[3] min */
	LONGS_EQUAL(0xffffff, target->Pc2SnesAdr(target, 0x3fffff));		/**< Slot[3] max */
	LONGS_EQUAL(ROMADDRESS_NULL, target->Pc2SnesAdr(target, 0x400000));	/**< overflow */

	/* Set SA1 address conversion mode */
	sa1inf.slots[0]		= 0x30;		/**< Slot[0] : PC $30:0000 - $3f:ffff */
	sa1inf.slots[1]		= 0x20;		/**< Slot[1] : PC $20:0000 - $2f:ffff */
	sa1inf.slots[2]		= 0x10;		/**< Slot[2] : PC $10:0000 - $1f:ffff */
	sa1inf.slots[3]		= 0x00;		/**< Slot[3] : PC $00:0000 - $0f:ffff */

	/*************** LoROM Address mode ***************/
	sa1inf.useHiRomMap	= false;	/**< Use LoROM Address conversion mode */
	target->SetSA1Info(target, sa1inf);
	LONGS_EQUAL(0x008000, target->Pc2SnesAdr(target, 0x300000));		/**< Slot[0] min */
	LONGS_EQUAL(0x1fffff, target->Pc2SnesAdr(target, 0x3fffff));		/**< Slot[0] max */
	LONGS_EQUAL(0x208000, target->Pc2SnesAdr(target, 0x200000));		/**< Slot[1] min */
	LONGS_EQUAL(0x3fffff, target->Pc2SnesAdr(target, 0x2fffff));		/**< Slot[1] max */
	LONGS_EQUAL(0x808000, target->Pc2SnesAdr(target, 0x100000));		/**< Slot[2] min */
	LONGS_EQUAL(0x9fffff, target->Pc2SnesAdr(target, 0x1fffff));		/**< Slot[2] max */
	LONGS_EQUAL(0xa08000, target->Pc2SnesAdr(target, 0x000000));		/**< Slot[3] min */
	LONGS_EQUAL(0xbfffff, target->Pc2SnesAdr(target, 0x0fffff));		/**< Slot[3] max */

	/*************** HiROM Address mode ***************/
	sa1inf.useHiRomMap	= true; 	/**< Use HiROM Address conversion mode */
	target->SetSA1Info(target, sa1inf);
	LONGS_EQUAL(0xc00000, target->Pc2SnesAdr(target, 0x300000));		/**< Slot[0] min */
	LONGS_EQUAL(0xcfffff, target->Pc2SnesAdr(target, 0x3fffff));		/**< Slot[0] max */
	LONGS_EQUAL(0xd00000, target->Pc2SnesAdr(target, 0x200000));		/**< Slot[1] min */
	LONGS_EQUAL(0xdfffff, target->Pc2SnesAdr(target, 0x2fffff));		/**< Slot[1] max */
	LONGS_EQUAL(0xe00000, target->Pc2SnesAdr(target, 0x100000));		/**< Slot[2] min */
	LONGS_EQUAL(0xefffff, target->Pc2SnesAdr(target, 0x1fffff));		/**< Slot[2] max */
	LONGS_EQUAL(0xf00000, target->Pc2SnesAdr(target, 0x000000));		/**< Slot[3] min */
	LONGS_EQUAL(0xffffff, target->Pc2SnesAdr(target, 0x0fffff));		/**< Slot[3] max */

	/*************** overflow check ***************/
	sa1inf.slots[0]		= 0x38;		/**< Slot[0] : PC $38:0000 - $47:ffff */

	/*--- Snes2Pc LoROM-like ---*/
	sa1inf.useHiRomMap	= false;	/**< Use LoROM Address conversion mode */
	target->SetSA1Info(target, sa1inf);
	LONGS_EQUAL(0x008000, target->Pc2SnesAdr(target, 0x380000));		/**< Slot[0] min */
	LONGS_EQUAL(0x0fffff, target->Pc2SnesAdr(target, 0x3fffff));		/**< Slot[0] max-size */
	LONGS_EQUAL(ROMADDRESS_NULL, target->Pc2SnesAdr(target, 0x400000));	/**< overflow */

	/*--- Snes2Pc HiROM-like ---*/
	sa1inf.useHiRomMap	= true; 	/**< Use HiROM Address conversion mode */
	target->SetSA1Info(target, sa1inf);
	LONGS_EQUAL(0xc00000, target->Pc2SnesAdr(target, 0x380000));		/**< Slot[0] min */
	LONGS_EQUAL(0xc7ffff, target->Pc2SnesAdr(target, 0x3fffff));		/**< Slot[0] max-size */
	LONGS_EQUAL(ROMADDRESS_NULL, target->Pc2SnesAdr(target, 0x400000));	/**< overflow */
}


/**
 * Check ExLoROM Address conversion
 */
TEST(RomFile_Address, ExLoROM)
{
	/* create check object */
	target = new_TestRomFile("L8192");
	LONGS_EQUAL(FileOpen_NoError, target->open(target));
	LONGS_EQUAL(RomType_ExLoRom, target->type_get(target));
	LONGS_EQUAL(MapMode_25, target->mapmode_get(target));

	/*--- Snes2Pc ---*/
	LONGS_EQUAL(0x000000, target->Snes2PcAdr(target, 0x808000));		/**< min(Lower 32MBits) */
	LONGS_EQUAL(0x400000, target->Snes2PcAdr(target, 0x008000));		/**< min(Upper 32MBits) */
	LONGS_EQUAL(0x3fffff, target->Snes2PcAdr(target, 0xffffff));		/**< max(Upper 32MBits) */
	LONGS_EQUAL(0x7effff, target->Snes2PcAdr(target, 0x7dffff));		/**< max(Lower 32MBits) */
	LONGS_EQUAL(0x200000, target->Snes2PcAdr(target, 0xc00000));		/**< HiRom/SA1 map ... (???) */
	LONGS_EQUAL(0x600000, target->Snes2PcAdr(target, 0x400000));		/**< HiRom/GSU map ... (???) */
	LONGS_EQUAL(ROMADDRESS_NULL, target->Snes2PcAdr(target, 0x7e8000));	/**< RAM */
	LONGS_EQUAL(ROMADDRESS_NULL, target->Snes2PcAdr(target, 0x000000));	/**< RAM(Mirror) */
	LONGS_EQUAL(ROMADDRESS_NULL, target->Snes2PcAdr(target, 0x007fff));	/**< EXT(Mirror) */

	/*--- Pc2Snes ---*/
	LONGS_EQUAL(0x808000, target->Pc2SnesAdr(target, 0x000000));		/**< min(High-Speed map) */
	LONGS_EQUAL(0xffffff, target->Pc2SnesAdr(target, 0x3fffff));		/**< max(High-Speed map) */
	LONGS_EQUAL(0x008000, target->Pc2SnesAdr(target, 0x400000));		/**< min(High-Speed map) */
	LONGS_EQUAL(0x7dffff, target->Pc2SnesAdr(target, 0x7effff));		/**< max(High-Speed map) */
	LONGS_EQUAL(ROMADDRESS_NULL, target->Pc2SnesAdr(target, 0x800000));	/**< overflow */
}


/**
 * Check ExHiROM Address conversion
 */
TEST(RomFile_Address, ExHiROM)
{
	/* create check object */
	target = new_TestRomFile("H8192");
	LONGS_EQUAL(FileOpen_NoError, target->open(target));
	LONGS_EQUAL(RomType_ExHiRom, target->type_get(target));
	LONGS_EQUAL(MapMode_25, target->mapmode_get(target));

	/*--- Snes2Pc ---*/
	LONGS_EQUAL(0x000000, target->Snes2PcAdr(target, 0xc00000));		/**< min */
	LONGS_EQUAL(0x400000, target->Snes2PcAdr(target, 0x400000));		/**< min(High-Speed map) */
	LONGS_EQUAL(0x3fffff, target->Snes2PcAdr(target, 0xffffff));		/**< max */
	LONGS_EQUAL(0x7dffff, target->Snes2PcAdr(target, 0x7dffff));		/**< max(High-Speed map) */
	LONGS_EQUAL(0x008000, target->Snes2PcAdr(target, 0x808000));		/**< LoRom map(High-speed map) */
	LONGS_EQUAL(0x408000, target->Snes2PcAdr(target, 0x008000));		/**< LoRom map */
	LONGS_EQUAL(0x0fffff, target->Snes2PcAdr(target, 0x8fffff));		/**< LoROM map max(High-Speed map) */
	LONGS_EQUAL(0x4fffff, target->Snes2PcAdr(target, 0x0fffff));		/**< LoROM map max */
	LONGS_EQUAL(ROMADDRESS_NULL, target->Snes2PcAdr(target, 0x7e8000));	/**< RAM */
	LONGS_EQUAL(ROMADDRESS_NULL, target->Snes2PcAdr(target, 0x000000));	/**< RAM(Mirror) */
	LONGS_EQUAL(ROMADDRESS_NULL, target->Snes2PcAdr(target, 0x007fff));	/**< EXT(Mirror) */

	/*--- Pc2Snes ---*/
	LONGS_EQUAL(0xc00000, target->Pc2SnesAdr(target, 0x000000));		/**< min(High-Speed map) */
	LONGS_EQUAL(0xffffff, target->Pc2SnesAdr(target, 0x3fffff));		/**< max(High-Speed map) */
	LONGS_EQUAL(0x400000, target->Pc2SnesAdr(target, 0x400000));		/**< min(High-Speed map) */
	LONGS_EQUAL(0x7dffff, target->Pc2SnesAdr(target, 0x7dffff));		/**< max(High-Speed map) */
	LONGS_EQUAL(ROMADDRESS_NULL, target->Pc2SnesAdr(target, 0x800000));	/**< overflow */
}
