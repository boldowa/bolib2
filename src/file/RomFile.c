/**
 * @file RomFile.c
 */
#include <assert.h>
#include <stdlib.h>
#include <memory.h>
#include "memory/bmem.h"
#include "file/RomFile.h"
#include "data/Str.h"

/* this header isn't read from anything other */
/* than inherited object.                     */ 
#include "RomFile.protected.h"
#include "File.protected.h"

#define FILL 0x00  /**< ROM fill byte */

typedef struct _RomTypeScore {
	bool hasHeader;
	bool detected;
	bool isSA1;
	bool isSPC7110;
} RomTypeScore;
typedef struct _RomDetectScore {
	bool header;
	RomTypeScore LoRom;
	RomTypeScore HiRom;
	RomTypeScore ExLoRom;
	RomTypeScore ExHiRom;
} RomDetectScore;


/* prototypes TODO : Fix it */
static void overrider_impl(RomFile* self);
static /*override*/ E_FileOpen open_impl(RomFile* self);
static /*override*/ void close_impl(RomFile* self);
static /*override*/ long size_get_impl(RomFile* self);
static RomType type_get_impl(RomFile* self);
static MapMode mapmode_get_impl(RomFile* self);
static uint16 sum_get_impl(RomFile* self);
static bool reload_impl(RomFile* self);
static bool write_impl(RomFile* self);
static bool IsValidSum_impl(RomFile* self);
static uint8* GetSnesPtr_impl(RomFile* self, const uint32 sna);
static uint8* GetPcPtr_impl(RomFile* self, const uint32 pca);
static uint32 RatsSearch_impl(RomFile* self, const uint32 sna, RatsSearcher_t search);
static uint32 RatsClean_impl(RomFile* self, const uint32 sna);
static void UseHiRomMapSA1_impl(RomFile* self, bool m);
static bool ChecksumUpdate_impl(RomFile* self);
static void DetectRomType_impl(RomFile* self);
static uint32 SearchFreeSpace_impl(RomFile* self, const uint32 len);
static bool HasHeader_impl(RomFile* self);

/* default handler */
static uint32 NullSnesAdr(RomFile* self, const uint32 ad);


/*--------------- Constructor / Destructor ---------------*/

/**
 * @brief Create RomFile object
 *
 * @return the pointer of object
 */
RomFile* new_RomFile_impl(const char* const path)
{
	File* super;
	RomFile* self;
	RomFile_protected* pro;

	/* make objects */
	super = new_File(path);
	self = calloc(1, sizeof(RomFile));
	pro = calloc(1, sizeof(RomFile_protected));

	/* check whether object creatin succeeded */
	assert(super);
	assert(pro);
	assert(self);

	/* inherit */
	memcpy(&self->super, super, sizeof(File));
	free(super);

	/* set default rom file mode */
	self->super.pro->mode = Str_copy("rb+");

	/*--- set protected member ---*/
	pro->overrider = overrider_impl;
	pro->hasHeader = false;
	pro->hcsum = pro->hcsumc = 0;
	pro->csum = 0;
	pro->map = MapMode_Unknown;
	pro->type = RomType_Unknown;
	pro->size = 0;
	pro->rom = NULL;
	pro->raw = NULL;
	pro->sa1adrinf.useHiRomMap = false;
	pro->sa1adrinf.slots[0] = 0;
	pro->sa1adrinf.slots[1] = 0;
	pro->sa1adrinf.slots[2] = 0;
	pro->sa1adrinf.slots[3] = 0;

	/*--- set public member ---*/
	self->type_get		= type_get_impl;
	self->mapmode_get	= mapmode_get_impl;
	self->sum_get		= sum_get_impl;
	self->reload		= reload_impl;
	self->write		= write_impl;
	self->IsValidSum	= IsValidSum_impl;
	self->GetSnesPtr	= GetSnesPtr_impl;
	self->GetPcPtr		= GetPcPtr_impl;
	self->Pc2SnesAdr	= NullSnesAdr;
	self->Snes2PcAdr	= NullSnesAdr;
	self->RatsSearch	= RatsSearch_impl;
	self->RatsClean		= RatsClean_impl;
	self->UseHiRomMapSA1	= UseHiRomMapSA1_impl;
	self->ChecksumUpdate	= ChecksumUpdate_impl;
	self->DetectRomType	= DetectRomType_impl;
	self->SearchFreeSpace	= SearchFreeSpace_impl;
	self->HasHeader		= HasHeader_impl;
	/* inherit */
	self->open		= open_impl;	/* override */
	self->close		= close_impl;	/* override */
	self->size_get		= size_get_impl;/* override */
	self->path_get		= (const char*(*)(RomFile*))self->super.path_get;
	self->dir_get		= (const char*(*)(RomFile*))self->super.dir_get;
	self->name_get		= (const char*(*)(RomFile*))self->super.name_get;
	self->ext_get		= (const char*(*)(RomFile*))self->super.ext_get;

	/* init RomFile object */
	self->pro = pro;
	self->pro->overrider(self);
	return self;
}

/**
 * @brief delete own member variables
 *
 * @param the pointer of object
 */
void delete_RomFile_members(RomFile* self)
{
	/* delete super members */
	self->close(self);
	delete_File_members(&self->super);

	/* delete protected members */
	free(self->pro);
}

/**
 * @brief Delete RomFile object
 *
 * @param the pointer of object
 */
void delete_RomFile_impl(RomFile** self)
{
	/* This is the template that default destractor. */
	assert(self);
	if(NULL == (*self)) return;
	delete_RomFile_members(*self);
	free(*self);
	(*self) = NULL;
}


/*--------------- internal methods ---------------*/
/* TODO : implement it */

static uint16 byteSum(const uint8 *data, long len)
{
	uint16 sum = 0;
	long i;

	for(i=0; i<len; i++)
	{
		sum = (uint16)(sum + data[i]);
	}

	return sum;
}
static void CalcSum(RomFile* self)
{
	long mask = 0x1000000;
	uint32 sumadr;

	assert(self);
	sumadr = self->Snes2PcAdr(self, 0xffdc);
	/*assert(ROMADDRESS_NULL != sumadr);*/
	if(ROMADDRESS_NULL == sumadr) return;

	/* get header sum */
	self->pro->hcsumc = read16(&self->pro->rom[sumadr+0]);
	self->pro->hcsum  = read16(&self->pro->rom[sumadr+2]);

	/* get first bit */
	while(0 == (mask & self->pro->size)) mask >>= 1;
	/* calc first bit sum */
	self->pro->csum = byteSum(&self->pro->rom[0], mask);
	/* calc other sums */
	self->pro->csum = (uint16)(self->pro->csum + (byteSum(&self->pro->rom[mask], self->pro->size-mask) << 1));
}


static void overrider_impl(RomFile* self)
{
	assert(self);

	self->super.path_get	= (const char*(*)(File*))self->path_get;
	self->super.dir_get	= (const char*(*)(File*))self->dir_get;
	self->super.name_get	= (const char*(*)(File*))self->name_get;
	self->super.ext_get	= (const char*(*)(File*))self->ext_get;
	self->super.open	= (E_FileOpen (*)(File*))self->open;
	self->super.close	= (void(*)(File*))self->close;
	self->super.size_get	= (long(*)(File*))self->size_get;
}

static /*override*/ E_FileOpen open_impl(RomFile* self)
{
	uint8* raw;
	E_FileOpen result;
#ifndef NDEBUG
	size_t rlen;
#endif

	assert(self);

	result = self->super.pro->open_vertual(&self->super);
	if(FileOpen_NoError != result)
	{
		return result;
	}

	raw = (uint8*)malloc((size_t)self->super.pro->size * sizeof(uint8));
	assert(raw);
#ifndef NDEBUG
	rlen =  fread(raw, sizeof(uint8), (size_t)self->super.pro->size, self->super.pro->fp);
#else
	fread(raw, sizeof(uint8), (size_t)self->super.pro->size, self->super.pro->fp);
#endif
	assert(self->super.pro->size == rlen);
	self->pro->raw = raw;

	self->DetectRomType(self);
	CalcSum(self);

	return FileOpen_NoError;
}

static /*override*/ void close_impl(RomFile* self)
{
	assert(self);
	free(self->pro->raw);
	self->pro->raw = NULL;
	self->pro->rom = NULL;
	self->super.pro->close_vertual(&self->super);
	self->pro->map = MapMode_Unknown;

	self->Snes2PcAdr = NullSnesAdr;
	self->Pc2SnesAdr = NullSnesAdr;
}


static /*override*/ long size_get_impl(RomFile* self)
{
	assert(self);
	return self->pro->size;
}


static RomType type_get_impl(RomFile* self)
{
	assert(self);
	return self->pro->type;
}


static MapMode mapmode_get_impl(RomFile* self)
{
	assert(self);
	return self->pro->map;
}


static uint16 sum_get_impl(RomFile* self)
{
	assert(self);
	return self->pro->csum;
}


static bool reload_impl(RomFile* self)
{
	assert(self);

	rewind(self->super.pro->fp);
	fseek(self->super.pro->fp, 0, SEEK_SET);
	return (self->super.pro->size == fread(self->pro->raw, sizeof(uint8), (size_t)self->super.pro->size, self->super.pro->fp));
}


static bool write_impl(RomFile* self)
{
	assert(self);

	/* re-calculate checksum */
	CalcSum(self);
	if(false == self->ChecksumUpdate(self)) return false;

	rewind(self->super.pro->fp);
	fseek(self->super.pro->fp, 0, SEEK_SET);
	if(self->pro->hasHeader)
	{
		fseek(self->super.pro->fp, 0x200, SEEK_SET);
	}
	return (self->pro->size == fwrite(self->pro->rom, sizeof(uint8), (size_t)self->pro->size, self->super.pro->fp));
}


static bool IsValidSum_impl(RomFile* self)
{
	assert(self);
	if(self->pro->hcsum == self->pro->csum)
	{
		if(self->pro->hcsum == (self->pro->hcsumc^0xffff))
		{
			return true;
		}
	}
	return false;
}


static uint8* GetSnesPtr_impl(RomFile* self, const uint32 sna)
{
	uint32 pca;

	assert(self);
	pca = self->Snes2PcAdr(self, sna);
	if(ROMADDRESS_NULL == pca) return NULL;

	return &self->pro->rom[pca];
}


static uint8* GetPcPtr_impl(RomFile* self, const uint32 pca)
{
	assert(self);
	if(ROMADDRESS_NULL == self->Pc2SnesAdr(self, pca)) return NULL;

	return &self->pro->rom[pca];
}


static uint32 RatsSearch_impl(RomFile* self, const uint32 sna, RatsSearcher_t search)
{
	uint32 adr;
	uint8* ptr;
	uint16 sz;
	uint16 szc;

	assert(self);
	if(MapMode_Unknown == self->pro->map) return ROMADDRESS_NULL;

	adr = self->Snes2PcAdr(self, sna);
	if(ROMADDRESS_NULL == adr) return ROMADDRESS_NULL;

	ptr = &self->pro->rom[adr];

	for ( ; ptr != NULL ; )
	{
		/* Search rats tag */
		ptr = bmsearch(ptr, (const uint8*)"STAR", 4, &self->pro->rom[self->pro->size]);
		if(NULL != ptr)
		{
			/* check valid rats tag */
			sz =  read16(ptr+4);
			szc = read16(ptr+6);
			if(sz == (szc^0xffff))
			{
				if(NULL == search || search(ptr+8, (uint32)(sz+1)))
				{
					return self->Pc2SnesAdr(self, (uint32)(ptr - self->pro->rom));
				}
			}

			/* unmatch */
			ptr += sz + 9;
		}
	}

	return ROMADDRESS_NULL;
}


static uint32 RatsClean_impl(RomFile* self, const uint32 sna)
{
	uint32 adr;
	uint8* ptr;
	uint16 sz;
	uint16 szc;

	assert(self);
	if(MapMode_Unknown == self->pro->map) return 0;

	/* Get rom address */
	adr = self->Snes2PcAdr(self, sna);
	if(ROMADDRESS_NULL == adr) return false;
	ptr = &self->pro->rom[adr];

	/* Check RATS tag */
	if(0 != memcmp(ptr, (const uint8*)"STAR", 4)) return false;

	sz =  read16(ptr+4);
	szc = read16(ptr+6);
	if(sz != (szc^0xffff)) return false;

	/* Fill data */
	sz = (uint16)(sz + 9);
	memset(ptr, FILL, sz);

	return (uint32)sz;
}


static void UseHiRomMapSA1_impl(RomFile* self, bool m)
{
	assert(self);
	self->pro->sa1adrinf.useHiRomMap = m;
}


static bool ChecksumUpdate_impl(RomFile* self)
{
	uint32 sumadr;
	uint16 csumc;

	assert(self);

	sumadr = self->Snes2PcAdr(self, 0xffdc);
	if(ROMADDRESS_NULL == sumadr) return false;

	csumc = self->pro->csum ^ 0xffff;
	write16(&self->pro->rom[sumadr+0], csumc);
	write16(&self->pro->rom[sumadr+2], self->pro->csum);
	self->pro->hcsum = self->pro->csum;
	self->pro->hcsumc = csumc;

	return true;
}




static const uint8* SearchSub(const uint8* begin, const uint8* end, const uint32 len, uint32* locContinue)
{
	uint32 i;
	uint32 c;
	uint32 base;
	uint16 rsize;

	c = 0;
	i = (*locContinue);
	base = 0;
	/* search loop */
	while(&begin[i]<end)
	{
		/* not zero data */
		if(0 != begin[i])
		{
			/* skip rats tag */
			if(0 == memcmp("STAR", &begin[i], 4))
			{
				rsize = read16(&begin[i+4]);
				if(0xffff == (rsize + read16(&begin[i+6])))
				{
					i = i + 8 + rsize;
				}
			}
			i++;
			base = i;
			c = 0;
			continue;
		}

		/* count zero */
		c++;
		if(len <= c)
		{
			return &begin[base];
		}
		i++;
	}

	(*locContinue) = i;
	return NULL;
}
static uint32 SearchFreeSpace_impl(RomFile* self, const uint32 len)
{
	uint32 reallen = len+8;
	uint8 *top, *tail;
	uint32 bnksize;
	const uint8 *result;
	uint32 locContinue;
	uint8 *end;

	if(0x800000>self->pro->size) return ROMADDRESS_NULL;

	end = self->GetPcPtr(self, (uint32)self->pro->size-1);
	bnksize = 0x8000;
	switch(self->pro->map)
	{
		case MapMode_21:
		case MapMode_21H:
		case MapMode_SPC7110:
			bnksize = 0x10000;
			break;

		case MapMode_25:
		case MapMode_25H:
			if(RomType_ExHiRom == self->pro->type)
				bnksize = 0x10000;
			break;

		default:
			break;
	}

	/* set range */
	top = self->GetPcPtr(self, 0x80000);
	tail = top + bnksize-1;

	/* search space */
	locContinue = 0;
	for(;tail<=end; top+=bnksize, tail+=bnksize, locContinue-=bnksize)
	{
		/* set range */
		result = SearchSub(top, tail, reallen, &locContinue);
		if(NULL != result)
		{
			uint32 pca;
			pca = (uint32)(result - self->GetPcPtr(self, 0));
			return self->Pc2SnesAdr(self, pca);
		}
	}

	return ROMADDRESS_NULL;
}


static bool HasHeader_impl(RomFile* self)
{
	return self->pro->hasHeader;
}


/*=== ROM address conversion functions ====================================*/

/*===== LoRom =====*/
static uint32 LoRom_Snes2Pc(RomFile* self, const uint32 sna)
{
	uint32 pca;
	if(0x1000000 <= sna) return ROMADDRESS_NULL;
	if(0 == (sna & 0x8000)) return ROMADDRESS_NULL;

	pca = (((sna&0x7f0000)>>1) + (sna&0x7fff));
	if(self->pro->size <= pca) return ROMADDRESS_NULL;
	return pca;
}
static uint32 LoRom_Pc2Snes(RomFile* self, const uint32 pca)
{
	if(self->pro->size <= pca) return ROMADDRESS_NULL;
	if(0x400000 <= pca) return ROMADDRESS_NULL;

	return ((((pca&0x7f8000)<<1)|0x800000) + ((pca&0x7fff)|0x8000));
}

/*===== SA-1(LoRom) =====*/

/**
 * RPG Hacker: New SA-1 mapping functions, borrowed from Asar, since original
 * functions didn't work correctly. See below for original functions.
 */
#ifdef ASAR_SA1ADDRMODE
/**
 * Note: This is the function for SMW-SA1.
 *       So, it can't use "UseHiRomMapSA1" method.
 */
static const int sa1banks[8] = { 0 << 20, 1 << 20, -1, -1, 2 << 20, 3 << 20, -1, -1 };

static uint32 SA1_Snes2Pc(RomFile* self, const uint32 sna)
{
	uint32 pca = 0xffffffff;

	if ((sna & 0x408000) == 0x008000)
	{
		pca = sa1banks[(sna & 0xE00000) >> 21] | ((sna & 0x1F0000) >> 1) | (sna & 0x007FFF);
	}
	if ((sna & 0xC00000) == 0xC00000)
	{
		pca = sa1banks[((sna & 0x100000) >> 20) | ((sna & 0x200000) >> 19)] | (sna & 0x0FFFFF);
	}

	/* valid address check */
	if(self->pro->size > pca) return pca;

	return ROMADDRESS_NULL;
}
static uint32 SA1_Pc2Snes(RomFile* self, const uint32 pca)
{
	int i; 
	for (i = 0;i < 8;i++)
	{
		if (sa1banks[i] == (pca & 0x600000))
		{
			return (uint32)(0x008000 | (i << 21) | ((pca&((i < 4) ? 0x0F8000 : 0x1F8000)) << 1) | (pca & 0x7FFF));
		}
	}
	return ROMADDRESS_NULL;
}
/**
 * RPG Hacker: Original SA-1 mapping functions.
 * Left here for reference.
 */

#else

/**
 * Any rom compatibility function.
 *   TODO: implement the function for switch slot.
 */
static uint32 SA1_Snes2Pc(RomFile* self, const uint32 sna)
{
	uint32 pca;
	int slot;
	uint8 bnk = (uint8)(sna >> 16);
	bool useHiRomMap;

	if((0x00 <= bnk) && (0x20 > bnk))
	{
		slot = 0;
		useHiRomMap = false;
	}
	else if((0x20 <= bnk) && (0x40 > bnk))
	{
		slot = 1;
		useHiRomMap = false;
	}
	else if((0x80 <= bnk) && (0xa0 > bnk))
	{
		slot = 2;
		useHiRomMap = false;
	}
	else if((0xa0 <= bnk) && (0xc0 > bnk))
	{
		slot = 3;
		useHiRomMap = false;
	}
	else if((0xc0 <= bnk) && (0xcf >= bnk))
	{
		slot = 0;
		useHiRomMap = true;
	}
	else if((0xd0 <= bnk) && (0xdf >= bnk))
	{
		slot = 1;
		useHiRomMap = true;
	}
	else if((0xe0 <= bnk) && (0xef >= bnk))
	{
		slot = 2;
		useHiRomMap = true;
	}
	else if((0xf0 <= bnk) && (0xff >= bnk))
	{
		slot = 3;
		useHiRomMap = true;
	}
	else
	{
		return ROMADDRESS_NULL;
	}

	if(useHiRomMap)
	{
		pca = (uint32)((((self->pro->sa1adrinf.slots[slot])+((uint32)bnk&0x0f)) << 16) + ((uint32)sna & 0xffff));
	}
	else
	{
		pca = (uint32)(((uint32)self->pro->sa1adrinf.slots[slot] << 16) + (((uint32)bnk&0x1f) << 15) + ((uint32)sna & 0x7fff));
	}

	if(self->pro->size <= pca) return ROMADDRESS_NULL;
	return pca;
}
static uint32 SA1_Pc2Snes(RomFile* self, const uint32 pca)
{
	int i;
	uint8 bnk = (uint8)(pca>>16);
	uint32 add = 0;

	if(self->pro->size <= pca) return ROMADDRESS_NULL;
	if(0x800000 <= pca) return ROMADDRESS_NULL;

	/* HiRom map */
	if(self->pro->sa1adrinf.useHiRomMap)
	{
		for(i=0; i<4; i++)
		{
			if(self->pro->sa1adrinf.slots[i] <= bnk
					&& self->pro->sa1adrinf.slots[i] + 16 > bnk)
			{
				break;
			}
		}
		if(4 <= i) return ROMADDRESS_NULL;
		return (((((self->pro->sa1adrinf.slots[i] & 0x7f) + ((uint32)bnk & 0x0f)) << 16) + (((uint32)pca&0xffff))) | 0xc00000);
	}

	/* LoRom map */
	for(i=0; i<4; i++)
	{
		if(self->pro->sa1adrinf.slots[i] <= bnk
		&& self->pro->sa1adrinf.slots[i] + 16 > bnk)
		{
			break;
		}
	}
	switch(i) /* switch slot */
	{
		case 0:
			add = 0x00;
			break;
		case 1:
			add = 0x20;
			break;
		case 2:
			add = 0x80;
			break;
		case 3:
			add = 0xa0;
			break;
		default:
			return ROMADDRESS_NULL;
	}
	return (((add + ((uint32)bnk & 0x0f)) << 16) + (((uint32)pca&0x17fff)|0x8000));
}
#endif

/*===== HiRom =====*/
static uint32 HiRom_Snes2Pc(RomFile* self, const uint32 sna)
{
	uint32 pca;

	if(0x1000000 <= sna) return ROMADDRESS_NULL;

	pca = sna & 0x3fffff;
	if(self->pro->size <= pca) return ROMADDRESS_NULL;
	return pca;
}
static uint32 HiRom_Pc2Snes(RomFile* self, const uint32 pca)
{
	if(self->pro->size <= pca) return ROMADDRESS_NULL;
	if(0x400000 <= pca) return ROMADDRESS_NULL;

	return pca | 0xc00000;
}

/*===== SPC7110(HiRom) =====*/
static uint32 SPC7110_Snes2Pc(RomFile* self, const uint32 sna)
{
	/* TODO: implement */
	return ROMADDRESS_NULL;
}
static uint32 SPC7110_Pc2Snes(RomFile* self, const uint32 pca)
{
	/* TODO: implement */
	return ROMADDRESS_NULL;
}

/*===== ExLoRom =====*/
static uint32 ExLoRom_Snes2Pc(RomFile* self, const uint32 sna)
{
	uint32 pca;

	if(0x1000000 <= sna) return ROMADDRESS_NULL;
	if(0 == (sna & 0x8000)) return ROMADDRESS_NULL;

	if(0 != (0x800000 & sna))
	{
		pca = (((sna&0x7f0000)>>1) + (sna&0x7fff));
	}
	else
	{
		pca = (((sna&0x7f0000)>>1) + (sna&0x7fff) + 0x400000);
	}
	if(self->pro->size <= pca) return ROMADDRESS_NULL;
	return pca;
}
static uint32 ExLoRom_Pc2Snes(RomFile* self, const uint32 pca)
{
	if(self->pro->size <= pca) return ROMADDRESS_NULL;
	if(0x7f0000 <= pca) return ROMADDRESS_NULL;

	if(0x400000 <= pca)
	{
		return ((((pca&0x7f8000)<<1)&0x7fffff) + ((pca&0x7fff)|0x8000));
	}
	return ((((pca&0x7f8000)<<1)|0x800000) + ((pca&0x7fff)|0x8000));
}

/*===== ExHiRom =====*/
static uint32 ExHiRom_Snes2Pc(RomFile* self, const uint32 sna)
{
	uint32 pca;

	if(0x1000000 <= sna) return ROMADDRESS_NULL;
	if(0 == (sna & 0x400000)) return ROMADDRESS_NULL;

	if(0xc00000 == (0xc00000 & sna))
	{
		pca = (sna & 0x3fffff);
	}
	else
	{
		if(0x3e0000 <= (sna&0x3fffff)) return ROMADDRESS_NULL;
		pca = (sna & 0x3fffff)+0x400000;
	}
	if(self->pro->size <= pca) return ROMADDRESS_NULL;
	return pca;
}
static uint32 ExHiRom_Pc2Snes(RomFile* self, const uint32 pca)
{
	if(self->pro->size <= pca) return ROMADDRESS_NULL;
	if(0x7e0000 <= pca) return ROMADDRESS_NULL;

	if(0x400000 > pca)
	{
		return pca | 0xc00000;
	}
	return ((pca & 0x3fffff) + 0x400000);
}


/*=== ROM type detection functions ====================================*/

static void ExRomJudge(RomFile* self, const uint32 pca, RomTypeScore* rts)
{
	uint8 map;

	map = self->pro->raw[pca - 0x40 + 0x15];

	/* check exrom */
	map &= 0x2f;
	if(0x25 == map)
	{
		rts->detected = true;
	}
}
static void HiRomJudge(RomFile* self, const uint32 pca, RomTypeScore* rts)
{
	uint8 map;

	map = self->pro->raw[pca - 0x40 + 0x15];

	/* check spc7110 (Far easter zero) */
	if(0x3a == map)
	{
		rts->detected = true;
		rts->isSPC7110 = true;
		return;
	}

	/* check generic hirom */
	map &= 0x2f;
	if(0x21 == map)
	{
		rts->detected = true;
	}
}
static void LoRomJudge(RomFile* self, const uint32 pca, RomTypeScore* rts)
{
	uint8 map;

	map = self->pro->raw[pca - 0x40 + 0x15];

	/* check sa-1 */
	if(0x23 == map)
	{
		rts->detected = true;
		rts->isSA1 = true;
		return;
	}

	/* check generic lorom */
	map &= 0x2f;
	if(0x20 == map)
	{
		rts->detected = true;
	}
}


static bool RomType_ValidSum(RomFile* self, const uint32 pca)
{
	uint16 hsum;
	uint16 hsumc;
	uint32 sumadr;

	if(self->super.pro->size < pca)
	{
		return false;
	}

	sumadr = pca - 0x24;
	hsumc = read16(&self->pro->raw[sumadr]);
	hsum  = read16(&self->pro->raw[sumadr+2]);

	if(0xffff != (hsumc + hsum))
	{
		return false;
	}

	return true;
}

static void DetectRomType_impl(RomFile* self)
{
	RomDetectScore rds = {0};

	/* Check has header */
	if(0x200 == (self->super.pro->size % 0x8000))
	{
		rds.header = true;
	}

	/* Check LoRom / SA1Rom */
	if(RomType_ValidSum(self, 0x8000))
	{
		LoRomJudge(self, 0x8000, &rds.LoRom);
	}
	if(true == rds.header)
	{
		if(RomType_ValidSum(self, 0x8200))
		{
			rds.LoRom.hasHeader = true;
			LoRomJudge(self, 0x8200, &rds.LoRom);
		}
	}

	/* Check HiRom / SPC7110Rom */
	if(RomType_ValidSum(self, 0x10000))
	{
		HiRomJudge(self, 0x10000, &rds.HiRom);
	}
	if(true == rds.header)
	{
		if(RomType_ValidSum(self, 0x10200))
		{
			rds.HiRom.hasHeader = true;
			HiRomJudge(self, 0x10200, &rds.HiRom);
		}
	}

	/* Check ExLoRom */
	if(RomType_ValidSum(self, 0x408000))
	{
		ExRomJudge(self, 0x408000, &rds.ExLoRom);
	}
	if(true == rds.header)
	{
		if(RomType_ValidSum(self, 0x408200))
		{
			rds.ExLoRom.hasHeader = true;
			ExRomJudge(self, 0x408200, &rds.ExLoRom);
		}
	}

	/* Check ExHiRom */
	if(RomType_ValidSum(self, 0x410000))
	{
		ExRomJudge(self, 0x410000, &rds.ExHiRom);
	}
	if(true == rds.header)
	{
		if(RomType_ValidSum(self, 0x410200))
		{
			rds.ExHiRom.hasHeader = true;
			ExRomJudge(self, 0x410200, &rds.ExHiRom);
		}
	}

	/* Init rom type */
	self->pro->type = RomType_Unknown;
	self->pro->hasHeader = false;
	self->pro->rom = &self->pro->raw[0];
	self->pro->size = self->super.pro->size;

	/* Jundge RomType */
	if(true == rds.ExHiRom.detected)
	{
		self->pro->map = self->pro->raw[0x40ffd5];
		self->pro->type = RomType_ExHiRom;
		self->Snes2PcAdr = ExHiRom_Snes2Pc;
		self->Pc2SnesAdr = ExHiRom_Pc2Snes;
		if(true == rds.ExHiRom.hasHeader)
		{
			self->pro->hasHeader = true;
			self->pro->map = self->pro->raw[0x4101d5];
			self->pro->rom = &self->pro->raw[0x200];
			self->pro->size -= 0x200;
		}
		return;
	}
	if(true == rds.ExLoRom.detected)
	{
		self->pro->map = self->pro->raw[0x407fd5];
		self->pro->type = RomType_ExLoRom;
		self->Snes2PcAdr = ExLoRom_Snes2Pc;
		self->Pc2SnesAdr = ExLoRom_Pc2Snes;
		if(true == rds.ExLoRom.hasHeader)
		{
			self->pro->hasHeader = true;
			self->pro->map = self->pro->raw[0x4081d5];
			self->pro->rom = &self->pro->raw[0x200];
			self->pro->size -= 0x200;
		}
		return;
	}
	if(true == rds.HiRom.detected)
	{
		self->pro->map = self->pro->raw[0xffd5];
		self->pro->type = RomType_HiRom;
		self->Snes2PcAdr = HiRom_Snes2Pc;
		self->Pc2SnesAdr = HiRom_Pc2Snes;
		if(true == rds.HiRom.hasHeader)
		{
			self->pro->hasHeader = true;
			self->pro->map = self->pro->raw[0x101d5];
			self->pro->rom = &self->pro->raw[0x200];
			self->pro->size -= 0x200;
		}
		if(true == rds.HiRom.isSPC7110)
		{
			self->pro->map = MapMode_SPC7110;
			self->Snes2PcAdr = SPC7110_Snes2Pc;
			self->Pc2SnesAdr = SPC7110_Pc2Snes;
		}
		return;
	}
	if(true == rds.LoRom.detected)
	{
		self->pro->map = self->pro->raw[0x7fd5];
		self->pro->type = RomType_LoRom;
		self->Snes2PcAdr = LoRom_Snes2Pc;
		self->Pc2SnesAdr = LoRom_Pc2Snes;
		if(true == rds.LoRom.hasHeader)
		{
			self->pro->hasHeader = true;
			self->pro->map = self->pro->raw[0x81d5];
			self->pro->rom = &self->pro->raw[0x200];
			self->pro->size -= 0x200;
		}
		if(true == rds.LoRom.isSA1)
		{
			self->Snes2PcAdr = SA1_Snes2Pc;
			self->Pc2SnesAdr = SA1_Pc2Snes;
			/* init sa-1 bankmap */
			self->pro->sa1adrinf.useHiRomMap = false;
			self->pro->sa1adrinf.slots[0] = 0x00;
			self->pro->sa1adrinf.slots[1] = 0x10;
			self->pro->sa1adrinf.slots[2] = 0x20;
			self->pro->sa1adrinf.slots[3] = 0x30;
		}
		return;
	}

	self->pro->rom = NULL;
}


/*=== Default handler ====================================*/
static uint32 NullSnesAdr(RomFile* self, const uint32 ad)
{
	return ROMADDRESS_NULL;
}
