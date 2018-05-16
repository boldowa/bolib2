/**
 * @file Str.c
 */
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "data/Str.h"

/**
 * @brief Copy string data.
 *
 * @param s source string
 *
 * @return malloced data
 */
char* Str_copy(const char* const s)
{
	char* cp;
	size_t len;

	if(NULL==s) return NULL;
	len = strlen(s);
	cp = calloc(len+1, sizeof(char));
	assert(cp);
	strcpy_s(cp, len+1, s);

	return cp;
}


/*--- (!)CAUTION(!) Don't edit this comment! ---*/

