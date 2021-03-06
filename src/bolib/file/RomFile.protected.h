/**
 * @file RomFile.protected.h
 */
#ifndef ROMFILE_PROTECTED_H
#define ROMFILE_PROTECTED_H

/**
 * RomFile main instance
 */
struct _RomFile_protected {
	bool		hasHeader;
	uint8*		raw;
	uint8*		rom;
	uint		size;
	RomType		type;
	MapMode		map;
	CopType		cop;
	uint16		csum;
	uint16		hcsum;
	uint16		hcsumc;
	SA1AdrInfo	sa1adrinf;	/* It simulates the SuperMMC. */

	/* override method */
	void (*overrider)(RomFile*);
};

/**
 * detete own members method prototype
 * I recommend it that you move this method in "RomFile.c"
 * if you plan to make the final class.
 */
void delete_RomFile_members(RomFile*);

#endif
