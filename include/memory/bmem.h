/**
 * @file bmem.h
 */
#ifndef BMEM_H
#define BMEM_H

#include "btypes.h"

#ifdef __cplusplus
extern "C" {
#endif

uint16 read16(const uint8 *data);

uint32 read24(const uint8 *data);

void write16(uint8* data, const uint16 val);

void write24(uint8* data, const uint32 val);

uint8* bmsearch(const uint8 *tgt, const uint8 *key, const int32 len, const uint8* end);

/*--- (!)CAUTION(!) Don't edit this comment! ---*/

#ifdef __cplusplus
}
#endif
#endif
