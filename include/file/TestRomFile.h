/**
 * @file TestRomFile.h
 */
#ifndef TESTROMFILE_H
#define TESTROMFILE_H

#include "btypes.h"
#include "file/File.h"
#include "file/RomFile.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * public accessor
 */
typedef struct _RomFile TestRomFile;
typedef struct _RomFile_protected TestRomFile_protected;
typedef struct _RomFile_private TestRomFile_private;

/**
 * Constructor
 */
TestRomFile* new_TestRomFile_impl(const char* const);
#define new_TestRomFile(a) (RomFile*)new_TestRomFile_impl(a)

/**
 * Destractor
 */
void delete_TestRomFile_impl(TestRomFile**);
#define delete_TestRomFile(a) delete_RomFile_impl((RomFile**)a)


#ifdef __cplusplus
}
#endif
#endif
