/**
 * @file Str.h
 */
#ifndef STR_H
#define STR_H

#include <btypes.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Multibyte functions */

char* Str_copy_A(const char* const);

char* Str_concat_A(const char* const, const char* const);

/* Unicode functions */

#if isWindows

WCHAR* Str_copy_W(const WCHAR* const);

WCHAR* Str_concat_W(const WCHAR* const, const WCHAR* const);

#endif

/*--- (!)CAUTION(!) Don't edit this comment! ---*/

/* function macros */
#if isWindows && defined(_UNICODE)
#  define Str_copy Str_copy_W
#  define Str_concat Str_concat_W
#else
#  define Str_copy Str_copy_A
#  define Str_concat Str_concat_A
#endif

#ifdef __cplusplus
}
#endif
#endif
