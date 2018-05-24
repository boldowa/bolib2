/**
 * @file MmlFile.protected.h
 */
#ifndef MMLFILE_PROTECTED_H
#define MMLFILE_PROTECTED_H

/**
 * MmlFile main instance
 *   TODO : Fix it
 */
struct _MmlFile_protected {
	const char* hello;
};

/**
 * detete own members method prototype
 * I recommend it that you move this method in "MmlFile.c"
 * if you plan to make the final class.
 */
void delete_MmlFile_members(MmlFile*);

#endif
