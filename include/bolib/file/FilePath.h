/**
 * @file FilePath.h
 */
#ifndef FILEPATH_H
#define FILEPATH_H

#include "bolib/btypes.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * public accessor
 */
typedef struct _FilePath FilePath;
typedef struct _FilePath_protected FilePath_protected;
typedef struct _FilePath_private FilePath_private;
struct _FilePath {
	/* getter */
	const char* (*path_get)(FilePath*);
	const char* (*dir_get)(FilePath*);
	const char* (*name_get)(FilePath*);
	const char* (*ext_get)(FilePath*);
	/* setter */
	bool (*path_set)(FilePath*, const char* const);
	bool (*dir_set)(FilePath*, const char* const);
	bool (*name_set)(FilePath*, const char* const);
	bool (*ext_set)(FilePath*, const char* const);
	/* clone method */
	FilePath* (*clone)(FilePath*);
	/* private members */
	FilePath_private* pri;
};

/**
 * Constructor
 */
FilePath* new_FilePath_impl(const char* const);
#define new_FilePath(a) new_FilePath_impl(a)

/**
 * Destractor
 */
void delete_FilePath_impl(FilePath**);
#define delete_FilePath(a) delete_FilePath_impl(a)


#ifdef __cplusplus
}
#endif
#endif
