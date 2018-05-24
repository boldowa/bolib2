/**
 * @file File.c
 * Abstruct file implements
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "file/File.h"
#include "file/FilePath.h"
#include "file/libfile.h"

/* this header isn't read from anything other */
/* than inherited object.                     */ 
#include "File.protected.h"

/* prototypes */
static void close_impl(File*);
static E_FileOpen open_impl(File*);
static long size_get_impl(File*);
static const char* path_get_impl(File*);
static const char* dir_get_impl(File*);
static const char* name_get_impl(File*);
static const char* ext_get_impl(File*);


/*--------------- Constructor / Destructor ---------------*/

/**
 * @brief Create File object
 *
 * @return the pointer of object
 */
File* new_File_impl(const char* const path)
{
	File* self;
	File_protected* pro;

	/* make objects */
	self = calloc(1, sizeof(File));
	pro = calloc(1, sizeof(File_protected));

	/* check whether object creatin succeeded */
	assert(pro);
	assert(self);

	/*--- set protected member ---*/
	pro->filePath = new_FilePath(path);
	assert(pro->filePath);
	pro->fp = NULL;
	pro->mode = NULL;
	pro->size = 0;

	/*--- set public member ---*/
	self->path_get = path_get_impl;
	self->dir_get = dir_get_impl;
	self->name_get = name_get_impl;
	self->ext_get = ext_get_impl;
	self->close = close_impl;
	self->open = open_impl;
	self->size_get = size_get_impl;

	/* init File object */
	self->pro = pro;
	return self;
}

/**
 * @brief delete own member variables
 *
 * @param the pointer of object
 */
void delete_File_members(File* self)
{
	/* delete protected members */
	free(self->pro->mode);
	delete_FilePath(&self->pro->filePath);
	if(NULL != self->pro->fp) fclose(self->pro->fp);
	free(self->pro);
}


/*
 * this function isn't used.
 */
#if 0
/**
 * @brief Delete File object
 *
 * @param the pointer of object
 */
void delete_File_impl(File** self)
{
	/* This is the template that default destractor. */
	assert(self);
	if(NULL == (*self)) return;
	delete_File_members(*self);
	free(*self);
	(*self) = NULL;
}
#endif


/*--------------- internal methods ---------------*/

/**
 * @brief Get file size
 *
 * @param self File object
 *
 * @return size of file
 */
static long size_get_impl(File* self)
{
	assert(self);
	return self->pro->size;
}


/**
 * @brief Close file
 *
 * @param self File object
 */
static void close_impl(File* self)
{
	assert(self);
	if(NULL != self->pro->fp)
	{
		fclose(self->pro->fp);
		self->pro->fp = NULL;
	}
}


/**
 * @brief Open file
 *
 * @param self File object
 *
 * @return FileOpen_NoError: Open OK / Others: failuer
 */
static E_FileOpen open_impl(File* self)
{
	File_protected* f;

	assert(self);
	f = self->pro;
	if(NULL != f->fp) return FileOpen_AlreadyOpen;
	if(NULL == f->mode) return FileOpen_NoMode;

	f->size = fsize(self->path_get(self));

	f->fp = fopen(self->path_get(self), f->mode);
	if(NULL == f->fp) return FileOpen_CantAccess;

	return FileOpen_NoError;
}


/**
 * @brief Get file path
 *
 * @param self File object
 *
 * @return File path
 */
static const char* path_get_impl(File* self)
{
	FilePath* fp;

	assert(self);
	fp = self->pro->filePath;
	return fp->path_get(fp);
}


/**
 * @brief Get file dir
 *
 * @param self File object
 *
 * @return File dir
 */
static const char* dir_get_impl(File* self)
{
	FilePath* fp;

	assert(self);
	fp = self->pro->filePath;
	return fp->dir_get(fp);
}


/**
 * @brief Get file name
 *
 * @param self File object
 *
 * @return File name
 */
static const char* name_get_impl(File* self)
{
	FilePath* fp;

	assert(self);
	fp = self->pro->filePath;
	return fp->name_get(fp);
}


/**
 * @brief Get file ext
 *
 * @param self File object
 *
 * @return File ext
 */
static const char* ext_get_impl(File* self)
{
	FilePath* fp;

	assert(self);
	fp = self->pro->filePath;
	return fp->ext_get(fp);
}

