/**
 * @file MmlFile.c
 */
#include <assert.h>
#include <stdlib.h>
#include <memory.h>
#include "file/MmlFile.h"

/* this header isn't read from anything other */
/* than inherited object.                     */ 
#include "MmlFile.protected.h"
#include "TextFile.protected.h"

/* Private member */
/* TODO : Fix it */
struct _MmlFile_private {
	int some_member;
};

/* prototypes TODO : Fix it */
/*static int some_method(MmlFile);*/


/*--------------- Constructor / Destructor ---------------*/

/**
 * @brief Create MmlFile object
 *
 * @return the pointer of object
 */
MmlFile* new_MmlFile_impl(const char* const path)
{
	TextFile* super;
	MmlFile* self;
	MmlFile_protected* pro;
	MmlFile_private* pri;

	/* make objects */
	super = new_TextFile(path);
	self = calloc(1, sizeof(MmlFile));
	pro = calloc(1, sizeof(MmlFile_protected));
	pri = calloc(1, sizeof(MmlFile_private));

	/* check whether object creatin succeeded */
	assert(pro);
	assert(pri);
	assert(self);

	/* inherit */
	memcpy(&self->super, super, sizeof(TextFile));
	free(super);

	/*--- set private member ---*/
	/* TODO : Fix it */
	pri->some_member = 0;

	/*--- set protected member ---*/
	/* TODO : Fix it */
	pro->hello = "";

	/*--- set public member ---*/
	/* TODO : Fix it */
	/* inherit TextFile */
	self->open = (E_FileOpen(*)(MmlFile*))self->super.open;
	self->open2 = (E_FileOpen(*)(MmlFile*, const char* const))self->super.open;
	self->row_get = (uint(*)(MmlFile*))self->super.row_get;
	self->getline = (const char*(*)(MmlFile*))self->super.getline;
	self->printf = (void(*)(MmlFile*, const char*, ...))self->super.printf;
	/* inherit File */
	self->path_get = (const char*(*)(MmlFile*))self->super.path_get;
	self->dir_get = (const char*(*)(MmlFile*))self->super.dir_get;
	self->name_get = (const char*(*)(MmlFile*))self->super.name_get;
	self->ext_get = (const char*(*)(MmlFile*))self->super.ext_get;
	self->close = (void(*)(MmlFile*))self->super.close;
	self->size_get = (long(*)(MmlFile*))self->super.size_get;

	/* init MmlFile object */
	self->pro = pro;
	self->pri = pri;
	return self;
}

/**
 * @brief delete own member variables
 *
 * @param the pointer of object
 */
void delete_MmlFile_members(MmlFile* self)
{
	/* delete super members */
	delete_TextFile_members(&self->super);

	/* delete protected members */
	free(self->pro);

	/* delete private members */
	free(self->pri);
}

/**
 * @brief Delete MmlFile object
 *
 * @param the pointer of object
 */
void delete_MmlFile_impl(MmlFile** self)
{
	/* This is the template that default destractor. */
	assert(self);
	if(NULL == (*self)) return;
	delete_MmlFile_members(*self);
	free(*self);
	(*self) = NULL;
}


/*--------------- internal methods ---------------*/

/* TODO : implement it */

