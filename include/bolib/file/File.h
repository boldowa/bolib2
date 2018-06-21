/**
 * @file File.h
 */
#ifndef FILE_H
#define FILE_H

#include "bolib/btypes.h"

#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief File open erroes
 */
typedef enum {
	FileOpen_NoError,
	FileOpen_AlreadyOpen,
	FileOpen_NoMode,
	FileOpen_CantAccess
} E_FileOpen;

/**
 * public accessor
 */
typedef struct _File File;
typedef struct _File_protected File_protected;
typedef struct _File_private File_private;
struct _File {
	const char* (*path_get)(File*);
	const char* (*dir_get)(File*);
	const char* (*name_get)(File*);
	const char* (*ext_get)(File*);
	E_FileOpen (*open)(File*);
	void (*close)(File*);
	uint (*size_get)(File*);

	/* protected members */
	File_protected* pro;
};

#ifdef __cplusplus
}
#endif
#endif
