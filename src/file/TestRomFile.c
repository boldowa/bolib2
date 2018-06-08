/**
 * @file TestRomFile.c
 *       RomFile class for Test
 */
#include <assert.h>
#include <stdlib.h>
#include <memory.h>
#include "memory/bmem.h"
#include "file/TestRomFile.h"

/* this header isn't read from anything other */
/* than inherited object.                     */ 
#include "RomFile.protected.h"
#include "File.protected.h"

#define FILL 0


/* prototypes */
static E_FileOpen dummyOpen(RomFile*);
static bool dummyReload(RomFile*);
static void dummyClose(RomFile*);
static bool dummyWrite(RomFile*);
static uint32 NullSnesAdr(RomFile*, const uint32);


/*--------------- Constructor / Destructor ---------------*/

/**
 * @brief Create TestRomFile object
 *
 * @return the pointer of object
 */
TestRomFile* new_TestRomFile_impl(const char* const str)
{
	TestRomFile* self;

	/* make objects */
	self = new_RomFile(NULL);
	assert(self);


	/*--- modify RomFile ---*/


	/* specify romtype from arg         */
	/* e.g. "l1024" : lorom, 1024Kbytes */
	switch(str[0])
	{
		case 'l':	/* lorom */
			self->pro->type = RomType_LoRom;
			self->pro->map = MapMode_20;
			break;

		case 's':	/* sa-1rom */
			self->pro->type = RomType_LoRom;
			self->pro->map = MapMode_SA1;
			break;

		case 'g':	/* SuperFXrom */
			self->pro->type = RomType_LoRom;
			self->pro->map = MapMode_20;
			self->pro->cop = CopType_SuperFX;
			break;

		case 'h':	/* hirom */
			self->pro->type = RomType_HiRom;
			self->pro->map = MapMode_21;
			break;

		case 'L':	/* exlorom */
			self->pro->type = RomType_ExLoRom;
			self->pro->map = MapMode_25;
			break;

		case 'H':	/* exhirom */
			self->pro->type = RomType_ExHiRom;
			self->pro->map = MapMode_25;
			break;

		default:
			self->pro->type = RomType_Unknown;
			self->pro->map = 0;
			break;
	}

	self->pro->size = (long)(atoi(&str[1]) * 1024); /* Kbytes */
	self->super.pro->size = self->pro->size;
	if(0x10000 > self->size_get(self))
	{
		delete_RomFile(&self);
		return NULL;
	}


	/*--- set public member ---*/
	/* modify op */
	self->open = dummyOpen;
	self->close = dummyClose;
	self->write = dummyWrite;
	self->reload = dummyReload;

	/* init TestRomFile object */
	self->pro->overrider(self);
	return self;
}


/*--------------- internal methods ---------------*/

static E_FileOpen dummyOpen(RomFile* self)
{
	uint8* raw;
	uint8 mb;
	uint32 mbits;
	uint32 headerBase;

	assert(self);
	
	mb = 0;
	mbits = 0x400;
	while(mbits < self->size_get(self))
	{
		mb++;
		mbits <<= 1;
	}

	raw = (uint8*)malloc((size_t)self->pro->size * sizeof(uint8));
	assert(raw);

	switch(self->mapmode_get(self))
	{
		case MapMode_20:
		case MapMode_20H:
			headerBase = 0x8000 - 0x40;
			break;

		case MapMode_SA1:
			headerBase = 0x8000 - 0x40;
			break;

		case MapMode_21:
		case MapMode_21H:
			headerBase = 0x10000 - 0x40;
			break;

		case MapMode_25:
		case MapMode_25H:
			assert(0x410000<self->size_get(self));
			switch(self->type_get(self))
			{
				case RomType_ExLoRom:
					headerBase = 0x408000 - 0x40;
					break;

				case RomType_ExHiRom:
					headerBase = 0x410000 - 0x40;
					break;

				case RomType_LoRom:
				case RomType_HiRom:
				case RomType_Unknown:
				default:
					break;
			}
			break;

		case MapMode_Reserved:
		case MapMode_SPC7110:
		case MapMode_Unknown:
		default:
			break;
	}

	/* Generate snes header */
	raw[headerBase+0x15] = self->mapmode_get(self);
	raw[headerBase+0x17] = mb;
	switch(self->pro->cop)
	{
		/* TODO: Other Co-processer */

		case CopType_SuperFX:
			raw[headerBase+0x16] = 0x15;
		default:
			break;
	}
	write16(&raw[headerBase+0x1c], 0x0000);
	write16(&raw[headerBase+0x1e], 0xffff);
	self->pro->raw = raw;

	/* Set address convert methods */
	self->DetectRomType(self);
#if 0
	/* debug printer */
	printf("RomType: 0x%02x\n", self->pro->type);
	printf("MapType: 0x%02x\n", self->pro->map);
	printf("CopType: 0x%02x\n", self->pro->cop);
#endif

	/* calculate checksum & update */
	self->ChecksumUpdate(self);

	return FileOpen_NoError;
}

static void dummyClose(RomFile* self)
{
	assert(self);
	free(self->pro->raw);
	self->pro->raw = NULL;
	self->pro->rom = NULL;

	self->Snes2PcAdr = NullSnesAdr;
	self->Pc2SnesAdr = NullSnesAdr;
}

static bool dummyReload(RomFile* self)
{
	assert(self);

	return false;
}

static bool dummyWrite(RomFile* self)
{
	assert(self);

	/* re-calculate checksum */
	return self->ChecksumUpdate(self);
}

/*=== Default methods ====================================*/
static uint32 NullSnesAdr(RomFile* self, const uint32 ad)
{
	return ROMADDRESS_NULL;
}

