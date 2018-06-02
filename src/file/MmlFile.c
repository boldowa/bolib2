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
static void overrider_impl(MmlFile* self);


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
	pro->overrider = overrider_impl;

	/*--- set public member ---*/
	/* TODO : Fix it */
	/* inherit TextFile */
	self->open = (E_FileOpen(*)(MmlFile*))self->super.open;
	self->open2 = (E_FileOpen(*)(MmlFile*, const char* const))self->super.open2;
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
	self->pro->overrider(self);
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

static void overrider_impl(MmlFile* self)
{
	assert(self);

	/* TextFile */
	self->super.open	= (E_FileOpen(*)(TextFile*))self->open;
	self->super.open2	= (E_FileOpen(*)(TextFile*, const char* const))self->open2;
	self->super.row_get	= (uint(*)(TextFile*))self->row_get;
	self->super.getline	= (const char*(*)(TextFile*))self->getline;
	self->super.printf	= (void(*)(TextFile*, const char*, ...))self->printf;
	/* File */
	self->super.path_get	= (const char*(*)(TextFile*))self->path_get;
	self->super.dir_get	= (const char*(*)(TextFile*))self->dir_get;
	self->super.name_get	= (const char*(*)(TextFile*))self->name_get;
	self->super.ext_get	= (const char*(*)(TextFile*))self->ext_get;
	self->super.close	= (void(*)(TextFile*))self->close;
	self->super.size_get	= (long(*)(TextFile*))self->size_get;

	self->super.pro->overrider(&self->super);
}

/* TODO : implement it */

