/**
 * @file MmlFile.h
 */
#ifndef MMLFILE_H
#define MMLFILE_H

#include "bolib/btypes.h"
#include "bolib/file/TextFile.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * public accessor
 *   TODO : fix it
 */
typedef struct _MmlFile MmlFile;
typedef struct _MmlFile_protected MmlFile_protected;
typedef struct _MmlFile_private MmlFile_private;
struct _MmlFile {
	/* inherited */
	TextFile super;
	/* member methods */
	/* ... */
	E_FileOpen (*open)(MmlFile*);
	E_FileOpen (*open2)(MmlFile*, const char*);
	uint (*row_get)(MmlFile*);
	const char* (*getline)(MmlFile*);
	void (*printf)(MmlFile*, const char*, ...);
	const char* (*path_get)(MmlFile*);
	const char* (*dir_get)(MmlFile*);
	const char* (*name_get)(MmlFile*);
	const char* (*ext_get)(MmlFile*);
	void (*close)(MmlFile*);
	long (*size_get)(MmlFile*);
	/* protected members */
	MmlFile_protected* pro;
	/* private members */
	MmlFile_private* pri;
};

/**
 * Constructor
 */
MmlFile* new_MmlFile_impl(const char* const);
#define new_MmlFile(a) new_MmlFile_impl(a)

/**
 * Destractor
 */
void delete_MmlFile_impl(MmlFile**);
#define delete_MmlFile(a) delete_MmlFile_impl(a)


#ifdef __cplusplus
}
#endif
#endif
