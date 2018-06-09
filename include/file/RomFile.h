/**
 * @file RomFile.h
 */
#ifndef ROMFILE_H
#define ROMFILE_H

#include <btypes.h>
#include "file/File.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Rom types
 */
typedef enum RomType{
	RomType_Unknown = 0,
	RomType_LoRom,
	RomType_HiRom,
	RomType_ExLoRom,
	RomType_ExHiRom
}RomType;

/**
 * @brief ROM map mpdes
 */
typedef enum MapMode{
	MapMode_Unknown = 0,
	/* Normal speed */
	MapMode_20	= 0x20,		/* LoRom */
	MapMode_21,			/* HiRom */
	MapMode_Reserved,
	MapMode_SA1,			/* SA-1 */
	MapMode_25	= 0x25,		/* ExHiRom */
	/* high speed */
	MapMode_20H	= 0x30,		/* LoRom */
	MapMode_21H,			/* HiRom */
	MapMode_25H	= 0x35,		/* ExHiRom */
	MapMode_SPC7110	= 0x3A		/* SPC7110 */

}MapMode;

typedef enum CopType {
	CopType_None	= 0,
	CopType_DSP,
	CopType_SuperFX,
	CopType_OBC1,
	CopType_SA1,
	CopType_Custom,
	CopType_Other
}CopType;

/**
 * @brief SA1 information
 */
typedef struct _SA1AdrInfo {
	bool		useHiRomMap;
	uint8		slots[4];		/**< Slot number(SuperMMC Bank) */
	bool		mbits[4];		/**< SuperMMC XBM bit */
} SA1AdrInfo;

/**
 * @brief Search data function pointer
 *
 * @param data ROM data address
 * @param size Rats data size
 *
 * @return true: match / false: unmatch
 */
typedef bool (*RatsSearcher_t)(const uint8* data, const uint32 size);


#define ROMADDRESS_NULL 0x80000000 /**< NULL of ROMAddress */


/**
 * public accessor
 */
typedef struct _RomFile RomFile;
typedef struct _RomFile_protected RomFile_protected;
typedef struct _RomFile_private RomFile_private;
struct _RomFile {
	/* inherited */
	File super;
	/* methods */
	RomType (*type_get)(RomFile*);
	MapMode (*mapmode_get)(RomFile*);
	uint16 (*sum_get)(RomFile*);
	bool (*reload)(RomFile*);
	bool (*write)(RomFile*);
	bool (*IsValidSum)(RomFile*);
	uint8* (*GetSnesPtr)(RomFile*, const uint32);
	uint8* (*GetPcPtr)(RomFile*, const uint32);
	uint32 (*Pc2SnesAdr)(RomFile*, const uint32);
	uint32 (*Snes2PcAdr)(RomFile*, const uint32);
	uint32 (*RatsSearch)(RomFile*, const uint32, RatsSearcher_t);
	uint32 (*RatsClean)(RomFile*, const uint32);
	void (*UseHiRomMapSA1)(RomFile*, bool);
	bool (*ChecksumUpdate)(RomFile*);
	void (*DetectRomType)(RomFile*);
	uint32 (*SearchFreeSpace)(RomFile*, const uint32);
	bool (*HasHeader)(RomFile*);
	void (*SetSA1Info)(RomFile*, SA1AdrInfo);
	SA1AdrInfo (*GetSA1Info)(RomFile*);
	/* ... */
	const char* (*path_get)(RomFile*);
	const char* (*dir_get)(RomFile*);
	const char* (*name_get)(RomFile*);
	const char* (*ext_get)(RomFile*);
	E_FileOpen (*open)(RomFile*);
	void (*close)(RomFile*);
	long (*size_get)(RomFile*);
	/* protected members */
	RomFile_protected* pro;
};

/**
 * Constructor
 */
RomFile* new_RomFile_impl(const char* const);
#define new_RomFile(a) new_RomFile_impl(a)

/**
 * Destractor
 */
void delete_RomFile_impl(RomFile**);
#define delete_RomFile(a) delete_RomFile_impl(a)


#ifdef __cplusplus
}
#endif
#endif
