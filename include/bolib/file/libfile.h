/**
 * @file libfile.h
 */
#ifndef LIBFILE_H
#define LIBFILE_H

#include "bolib/btypes.h"

#ifdef __cplusplus
extern "C" {
#endif

bool fexists(const char* const);

bool dexists(const char* const);

char* gettmpdir(void);

char* abspath(const char* const);

long fsize(const char* const);

bool makedir(const char* const);

/*--- (!)CAUTION(!) Don't edit this comment! ---*/

#ifdef __cplusplus
}
#endif
#endif
