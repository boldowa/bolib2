/**
 * @file TextFile.protected.h
 */
#ifndef TEXTFILE_PROTECTED_H
#define TEXTFILE_PROTECTED_H

/**
 * TextFile main instance
 */
struct _TextFile_protected {
	/* member */
	char* lineBuffer;
	uint line;
};

/**
 * detete own members method prototype
 * I recommend it that you move this method in "TextFile.c"
 * if you plan to make the final class.
 */
void delete_TextFile_members(TextFile*);

#endif
