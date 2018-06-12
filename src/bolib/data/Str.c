/**
 * @file Str.c
 */
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "bolib/data/Str.h"

/**
 * @brief Copy string data function template
 *
 * @param FuncName function name
 * @param T data type
 * @param LenFunc strlen function
 * @param CpyFunc strcpy function
 *
 * @return string data(alloced)
 */
#define Tmpl_Str_copy(FuncName, T, LenFunc, CpyFunc) \
T* FuncName(const T* const s)\
{\
	T* cp;\
	size_t len;\
\
	if(NULL==s) return NULL;\
	len = LenFunc(s);\
	cp = calloc(len+1, sizeof(T));\
	assert(cp);\
	CpyFunc(cp, len+1, s);\
\
	return cp;\
}

/**
 * @brief Concat string data function template
 *
 * @param FuncName function name
 * @param T data type
 * @param LenFunc strlen function
 * @param CpyFunc strcpy function
 *
 * @return string data(alloced)
 */
#define Tmpl_Str_concat(FuncName, T, LenFunc, CpyFunc)\
T* FuncName(const T* const s1, const T* const s2)\
{\
	T* con;\
	T* top;\
	size_t len;\
	size_t s1len;\
\
	if(NULL == s1 || NULL == s2) return NULL;\
\
	/* Memory allocation */\
	len = LenFunc(s1) + LenFunc(s2);\
	con = calloc(len+1, sizeof(T));\
	assert(con);\
	top = con;\
\
	/* Connect string */\
	CpyFunc(con, len+1, s1);\
	s1len = LenFunc(s1);\
	con += s1len;\
	len -= s1len;\
	CpyFunc(con, len+1, s2);\
\
	return top;\
}

/**
 * @brief Copy string data
 *
 * @param s source string
 *
 * @return malloced data
 */
Tmpl_Str_copy(Str_copy_A, char, strlen, strcpy_s)
/* char* Str_copy_A(const char* const s) */


/**
 * @brief Connect strings
 *
 * @param s1 strings
 * @param s2 strings
 *
 * @return connected strings(malloced)
 */
Tmpl_Str_concat(Str_concat_A, char, strlen, strcpy_s)
/* char* Str_concat_A(const char* const s1, const char* const s2)*/


#if isWindows

/**
 * @brief Copy string data(UNICODE)
 *
 * @param s source string
 *
 * @return malloced data
 */
Tmpl_Str_copy(Str_copy_W, WCHAR, wcslen, wcscpy_s)
/* WCHAR* Str_copy_A(const WCHAR* const s) */


/**
 * @brief Connect strings(UNICODE)
 *
 * @param s1 strings
 * @param s2 strings
 *
 * @return connected strings(malloced)
 */
Tmpl_Str_concat(Str_concat_W, WCHAR, wcslen, wcscpy_s)
/* WCHAR* Str_concat_A(const WCHAR* const s1, const WCHAR* const s2)*/

#endif

/**
 * @brief TODO: Write summary.
 */
void Str_tolower(char* s)
{
	if(NULL==s) return;
	for(; '\0' != *s; s++)
	{
		*s = (char)tolower(*s);
	}
}


/**
 * @brief TODO: Write summary.
 */
void Str_toupper(char* s)
{
	if(NULL==s) return;
	for(; '\0' != *s; s++)
	{
		*s = (char)toupper(*s);
	}
}


/**
 * @brief TODO: Write summary.
 */
char* Str_replace(const char* search, const char* replace, const char* target)
{
	char* work = NULL;
	char* top = NULL;
	char* tmp = NULL;
	char* tmp2 = NULL;
	char* find = NULL;
	char* result;
	size_t len;

	if(NULL == search || NULL == replace || NULL == target) return NULL;

	/* Empty values fail */
	if(0 == strlen(search)) return NULL;

	/* Init result value(default is "") */
	result = calloc(1, sizeof(char));
	assert(result);

	/* Copy for work */
	len = strlen(target);
	work = calloc(len+1, sizeof(char));
	assert(work);
	strcpy_s(work, len+1, target);
	top = work;
	
	/* Replacement loop */
	while(NULL != (find = strstr(work, search)))
	{
		/* Insert termination code on find point */
		*find = '\0';

		/* Replace 1 : connect with first half */
		tmp = Str_concat(work, replace);
		work = find;
		work += strlen(search);

		/* Replace 2 : connect with second half */
		tmp2 = Str_concat(result, tmp);
		free(tmp);
		free(result);
		result = tmp2;
		tmp2 = NULL;
	}

	/* connect with remainder */
	tmp = Str_concat(result, work);

	/* clean */
	free(result);
	free(top);

	/* return result */
	return tmp;
}


/*--- (!)CAUTION(!) Don't edit this comment! ---*/

