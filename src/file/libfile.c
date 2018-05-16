/**
 * @file libfile.c
 */
#if !isWindows
#  ifndef _XOPEN_SOURCE
#    define _XOPEN_SOURCE 500
#  endif
#endif

#include <stdio.h>
#include <assert.h>
#if !isWindows
#  include <sys/stat.h>
#endif
#include "data/Str.h"
#include "file/libfile.h"

/**
 * @brief Tests the file exists
 *
 * @param path file path
 *
 * @return true: exists / false: not exixts
 */
bool fexists(const char* const path)
{
#if isWindows

	HANDLE hFile;

	hFile = CreateFile(
			path,
			GENERIC_READ, FILE_SHARE_READ,
			NULL,
			OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL,
			NULL
			);
	if(INVALID_HANDLE_VALUE != hFile)
	{
		CloseHandle(hFile);
		return true;
	}
	return false;

#else

	struct stat sts;

	if(0 == stat(path, &sts) && S_ISREG(sts.st_mode))
	{
		return true;
	}
	return false;

#endif
}


/**
 * @brief Tests the directory exists
 *
 * @param path directory path
 *
 * @return true: exists / false: not exixts
 */
bool dexists(const char* const path)
{
#if isWindows
	DWORD attr;

	attr = GetFileAttributes(path);
	if((attr != INVALID_FILE_ATTRIBUTES) && (0 != (attr & FILE_ATTRIBUTE_DIRECTORY)))
	{
		return true;
	}
	return false;
#else
	struct stat sts;

	if(0 == stat(path, &sts) && S_ISDIR(sts.st_mode))
	{
		return true;
	}
	return false;
#endif
}


/**
 * @brief Get system temporary directory path
 *
 * @return Temp directory path(malloced, you need to free() it!)
 */
char* gettmpdir(void)
{
	char* s;
#if isWindows
	s = calloc(sizeof(char), MAX_PATH);
	GetTempPath(MAX_PATH, s);
	assert(s);
#else
	s = Str_copy("/tmp/");
	assert(s);
#endif
	return s;
}


/**
 * @brief Get path's absolute path
 *
 * @param path relative path
 *
 * @return absolute path(malloced, you need to free() it!)
 */
char* abspath(const char* const path)
{
	return NULL;
}


/**
 * @brief Gets the file size
 *
 * @param path file path
 *
 * @return file size
 */
long fsize(const char* const path)
{
#if isWindows
	HANDLE hFile;
	DWORD dwSize;

	hFile = CreateFile(path,
		GENERIC_READ, FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL,
		NULL);
	if(INVALID_HANDLE_VALUE == hFile) return 0;

	dwSize = GetFileSize(hFile, NULL);
	CloseHandle(hFile);
	return (long)dwSize;
#else
	struct stat st;
	if(0 == stat(path, &st) && S_ISREG(st.st_mode))
	{
		return st.st_size;
	}
	return 0;
#endif
}


/**
 * @brief Make the directory
 *
 * @param path directory path
 *
 * @return true: succeed / false: failure
 */
bool makedir(const char* const path)
{
#if isWindows
	return (0 != CreateDirectory(path, NULL));
#else
	return (0 == mkdir(path, 0755));
#endif
}


/*--- (!)CAUTION(!) Don't edit this comment! ---*/

