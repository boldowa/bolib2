/**
 * @file TextFile.c
 */
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include <stdarg.h>
#include "bolib/file/TextFile.h"
#include "bolib/data/Str.h"

/* this header isn't read from anything other */
/* than inherited object.                     */ 
#include "TextFile.protected.h"
#include "File.protected.h"

#define BuffLen 256

/* prototypes */
static E_FileOpen open_impl(TextFile*);
static E_FileOpen open2_impl(TextFile*, const char*);
static uint row_get_impl(TextFile*);
static const char* getline_impl(TextFile*);
static void printf_impl(TextFile*, const char*, ...);
static void overrider_impl(TextFile* self);


/*--------------- Constructor / Destructor ---------------*/

/**
 * @brief Create TextFile object
 *
 * @return the pointer of object
 */
TextFile* new_TextFile_impl(const char* const path)
{
	File* super;
	TextFile* self;
	TextFile_protected* pro;

	/* make objects */
	super = new_File(path);
	pro = malloc(sizeof(TextFile_protected));
	self = malloc(sizeof(TextFile));

	/* check whether object creatin succeeded */
	assert(pro);
	assert(self);

	/* inherit */
	memcpy(&self->super, super, sizeof(File));
	free(super);

	/*--- set protected member ---*/
	pro->line = 0;
	pro->lineBuffer = NULL;
	pro->overrider = overrider_impl;

	/*--- set public member ---*/
	self->open = open_impl;
	self->open2 = open2_impl;
	self->row_get = row_get_impl;
	self->getline = getline_impl;
	self->printf = printf_impl;
	/* inherit */
	self->path_get = (const char*(*)(TextFile*))self->super.path_get;
	self->dir_get = (const char*(*)(TextFile*))self->super.dir_get;
	self->name_get = (const char*(*)(TextFile*))self->super.name_get;
	self->ext_get = (const char*(*)(TextFile*))self->super.ext_get;
	self->close = (void(*)(TextFile*))self->super.close;
	self->size_get = (uint(*)(TextFile*))self->super.size_get;

	/* init TextFile object */
	self->pro = pro;
	self->pro->overrider(self);
	return self;
}

/**
 * @brief delete own member variables
 *
 * @param the pointer of object
 */
void delete_TextFile_members(TextFile* self)
{
	/* delete super members */
	delete_File_members(&self->super);

	/* delete protected members */
	free(self->pro->lineBuffer);

	/* delete protected members */
	free(self->pro);
}

/**
 * @brief Delete TextFile object
 *
 * @param the pointer of object
 */
void delete_TextFile_impl(TextFile** self)
{
	/* This is the template that default destractor. */
	assert(self);
	if(NULL == (*self)) return;
	delete_TextFile_members(*self);
	free(*self);
	(*self) = NULL;
}


/*--------------- internal methods ---------------*/

static void overrider_impl(TextFile* self)
{
	assert(self);

	self->super.path_get	= (const char*(*)(File*))self->path_get;
	self->super.dir_get	= (const char*(*)(File*))self->dir_get;
	self->super.name_get	= (const char*(*)(File*))self->name_get;
	self->super.ext_get	= (const char*(*)(File*))self->ext_get;
	self->super.open	= (E_FileOpen (*)(File*))self->open;
	self->super.close	= (void(*)(File*))self->close;
	self->super.size_get	= (uint(*)(File*))self->size_get;
}

static uint row_get_impl(TextFile* self)
{
	assert(self);
	return self->pro->line;
}

static const char* getline_impl(TextFile* self)
{
	char buf[BuffLen];
	char* result;
	char* tmp;
	char* p;
	size_t chompInx;
	TextFile_protected* txt;
	File_protected* filep;

	assert(self);
	txt = self->pro;
	filep = self->super.pro;

	result = calloc(1, sizeof(char));
	assert(result);

	while(true)
	{
		p = fgets(buf, BuffLen, filep->fp);
		if(NULL == p)
		{
			if(0 == strlen(result))
			{
				free(result);
				free(txt->lineBuffer);
				txt->lineBuffer = NULL;
				return NULL;
			}
			break;
		}

		tmp = Str_concat(result, buf);
		assert(tmp);
		free(result);
		result = tmp;
		tmp = NULL;

		if('\n' == buf[strlen(buf)-1])
		{
			break;
		}
	}

	/* chomp */
	chompInx = strlen(result) -1;
	while( ('\n' == result[chompInx]) || ('\r' == result[chompInx]))
	{
		result[chompInx--] = '\0';
	}

	free(txt->lineBuffer);
	txt->lineBuffer = result;
	txt->line++;
	return result;
}

static void printf_impl(TextFile* self, const char* fmt, ...)
{
	FILE* fp;
	va_list vl;

	assert(self);
	fp = self->super.pro->fp;
	va_start(vl, fmt);
	vfprintf(fp, fmt, vl);
	va_end(vl);
}

static E_FileOpen open2_impl(TextFile* self, const char* mode)
{
	File_protected* filep;
	TextFile_protected* textp;

	assert(self);

	filep = self->super.pro;
	textp = self->pro;
	if(NULL != mode)
	{
		if(NULL != filep->mode)
		{
			free(filep->mode);
		}
		filep->mode = Str_copy(mode);
		assert(filep->mode);
	}

	textp->line = 0;
	return self->super.pro->open_vertual(&self->super);
}

static E_FileOpen open_impl(TextFile* self)
{
	return open2_impl(self, "r");   /* read only */
}

