/**
 * @file File.protected.h
 */
#ifndef FILE_PROTECTED_H
#define FILE_PROTECTED_H

#include <stdio.h>
#include "file/FilePath.h"

/**
 * File main instance
 */
struct _File_protected {
	/* File path */
	FilePath* filePath;
	/* File pointer */
	FILE *fp;
	/* File mode */
	char* mode;
	/* File size */
	long size;

	/* vertual function pointer */
	E_FileOpen (*open_vertual)(File*);
	void (*close_vertual)(File*);
};

/**
 * detete own members method prototype
 * I recommend it that you move this method in "File.c"
 * if you plan to make the final class.
 */
void delete_File_members(File*);


/*
 * hide constructor and destructor from not inherited source
 */

/**
 * Constructor
 */
File* new_File_impl(const char* const);
#define new_File(a) new_File_impl(a)

/**
 * Destractor
 * this function isn't used.
 */
#if 0
void delete_File_impl(File**);
#define delete_File(a) delete_File_impl(a)
#endif


#endif
