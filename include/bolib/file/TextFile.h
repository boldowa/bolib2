/**
 * @file TextFile.h
 */
#ifndef TEXTFILE_H
#define TEXTFILE_H

#include "bolib/btypes.h"
#include "bolib/file/File.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * public accessor
 */
typedef struct _TextFile TextFile;
typedef struct _TextFile_protected TextFile_protected;
typedef struct _TextFile_private TextFile_private;
struct _TextFile {
	/* inherited */
	File super;
	/* member method */
	E_FileOpen (*open)(TextFile*);
	E_FileOpen (*open2)(TextFile*, const char*);
	uint (*row_get)(TextFile*);
	const char* (*getline)(TextFile*);
	void (*printf)(TextFile*, const char*, ...);
	/* ... */
	const char* (*path_get)(TextFile*);
	const char* (*dir_get)(TextFile*);
	const char* (*name_get)(TextFile*);
	const char* (*ext_get)(TextFile*);
	void (*close)(TextFile*);
	long (*size_get)(TextFile*);
	/* protected members */
	TextFile_protected* pro;
};

/**
 * Constructor
 */
TextFile* new_TextFile_impl(const char* const);
#define new_TextFile new_TextFile_impl

/**
 * Destractor
 */
void delete_TextFile_impl(TextFile**);
#define delete_TextFile delete_TextFile_impl


#ifdef __cplusplus
}
#endif
#endif
