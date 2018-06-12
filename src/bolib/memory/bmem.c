/**
 * @file bmem.c
 */
#include "bolib/memory/bmem.h"

/**
 * @brief Read 16bits value from memory.
 *
 * @param data binary data address
 *
 * @return 16bits value
 */
uint16 read16(const uint8 *data)
{
	return (uint16)(data[0] + ((uint16)(data[1]) << 8));
}


/**
 * @brief Read 24bits value from memory.
 *
 * @param data binary data address
 *
 * @return 24bits value
 */
uint32 read24(const uint8 *data)
{
	return (uint32)(data[0] + ((uint32)(data[1]) << 8) + ((uint32)(data[2]) << 16));
}


/**
 * @brief Write 16bits value into memory.
 *
 * @param data binary data address
 * @param val 16bits value
 */
void write16(uint8* data, const uint16 val)
{
	data[0] = (uint8)(val & 0xff);
	data[1] = (uint8)(val >> 8);
}


/**
 * @brief Write 24bits value into memory.
 *
 * @param data binary data address
 * @param val 24bits value
 */
void write24(uint8* data, const uint32 val)
{
	data[0] = (uint8)(val & 0xff);
	data[1] = (uint8)((val >> 8) & 0xff);
	data[2] = (uint8)(val >> 16);
}


/**
 * @brief Search binary data from memory
 *
 * @param tgt search start memory address
 * @param key key data address
 * @param len key data length
 * @param end search end memory address
 *
 * @return NULL: Not found / NOT NULL: Found data address
 */
uint8* bmsearch(const uint8 *tgt, const uint8 *key, const int32 len, const uint8* end)
{
	int32 tail;
	int32 shifts[256];
	int i;

	/* generate shift table */
	for(i=0; i<256; i++) shifts[i] = len;
	for(i=len-1;i>=0;i--)
	{
		shifts[key[i]] = len-i-1;
	}

	/* search */
	tail = len-1;
	while(&tgt[tail] <= end)
	{
		/* skip until tail match */
		i = tail;
		if(tgt[i] != key[i])
		{
			tgt = tgt + shifts[tgt[i]];
			continue;
		}

		/* search from tail */
		for(i--; i>=0; i--)
		{
			if(tgt[i] != key[i]) break;
		}

		/* match all data, found */
		if(i<0) return (uint8*)tgt;

		/* not match, increase pointer */
		if(0 != shifts[tgt[i+1]])
		{
			tgt = tgt + shifts[tgt[i+1]];
		}
		else
		{
			tgt++;
		}
	}
	return NULL;
}


/*--- (!)CAUTION(!) Don't edit this comment! ---*/

