/**
 * @file Vector.c
 */
#include <assert.h>
#include <stdlib.h>
#include <memory.h>
#include "bolib/data/Vector.h"

/* this header isn't read from anything other */
/* than inherited object.                     */ 
#include "Vector.protected.h"

/* prototypes */
static bool default_cloner_impl(void*, const void* const, size_t);
static const void* const get_impl(Vector*, size_t);
static bool set_impl(Vector*, size_t, const void* const);
static size_t length_impl(Vector*);
static size_t memlen_impl(Vector*);
static bool append_impl(Vector*, const void* const, size_t);
static bool insert_impl(Vector*, const size_t, const void* const, size_t);
static size_t remove_impl(Vector*, const size_t, const size_t);
static bool clear_impl(Vector*);
static bool connect_impl(Vector*, const Vector* const);
static bool alloc_impl(Vector*, size_t);
static Vector* clone_impl(const Vector* const);


/*--------------- Constructor / Destructor ---------------*/

/**
 * @brief Create Vector object
 *
 * @return the pointer of object
 */
Vector* new_Vector_impl(size_t item_size, size_t threshold, VectorDataCloner_t cloner, VectorDataDeleter_t deleter, size_t max, size_t min)
{
	Vector* self;
	Vector_protected* pro;
	void* data;
	size_t sz;

	/* parameter check */
	assert(item_size);
	assert((0!=threshold)&&(0!=min));
	assert(max>=min||max==0);

	sz = threshold;
	if(min>sz) sz=min;

	/* make objects */
	self = calloc(1, sizeof(Vector));
	pro = calloc(1, sizeof(Vector_protected));
	data = calloc(sz, item_size);

	/* check whether object creatin succeeded */
	assert(pro);
	assert(self);
	assert(data);

	/*--- set protected member ---*/
	pro->data = data;
	pro->alloc_size = sz;
	pro->length = 0;
	pro->item_size = item_size;
	pro->minimum_size = min;
	pro->max_size = max;
	pro->realloc_threshold_length = threshold;
	pro->cloner = cloner;
	pro->deleter = deleter;
	if(NULL==pro->cloner) pro->cloner = default_cloner_impl;

	/*--- set public member ---*/
	self->get = get_impl;
	self->set = set_impl;
	self->length = length_impl;
	self->memlen = memlen_impl;
	self->append = append_impl;
	self->insert = insert_impl;
	self->remove = remove_impl;
	self->clear = clear_impl;
	self->connect = connect_impl;
	self->alloc = alloc_impl;
	self->clone = clone_impl;

	/* init Vector object */
	self->pro = pro;
	return self;
}

/**
 * @brief Delete all vector data
 *
 * @param self Vector object
 */
static void cleanup_data(Vector* self, size_t beg, size_t len)
{
	size_t i;
	uint8* ofs;

	ofs = (uint8*)self->pro->data + (beg*self->pro->item_size);
	for(i=0; i<len; i++)
	{
		if(NULL!=self->pro->deleter)
			self->pro->deleter(ofs + (i*self->pro->item_size));
	}
}

/**
 * @brief delete own member variables
 *
 * @param the pointer of object
 */
void delete_Vector_members(Vector* self)
{
	/* delete protected members */
	cleanup_data(self, 0, self->pro->length);
	free(self->pro->data);
	free(self->pro);
}

/**
 * @brief Delete Vector object
 *
 * @param the pointer of object
 */
void delete_Vector_impl(Vector** self)
{
	/* This is the template that default destractor. */
	assert(self);
	if(NULL == (*self)) return;
	delete_Vector_members(*self);
	free(*self);
	(*self) = NULL;
}


/*--------------- internal methods ---------------*/

/**
 * @brief Fill memory by zero.
 *
 * @param self Vector object
 * @param beg fill start offset
 * @param end fill end offset
 */
static void fill_memory(Vector* self, size_t beg, size_t end)
{
	uint8* ptr;
	assert(self);

	ptr = self->pro->data;
	memset(ptr+(self->pro->item_size*beg), 0, (self->pro->item_size*(end-beg)));
}


/**
 * @brief Allocate memory
 *
 * @param self Vector object
 * @param size Allocate size
 *
 * @return true: succeed / false: failure
 */
static bool alloc_impl(Vector* self, size_t size)
{
	size_t current;
	size_t sz;
	void* new_data;

	assert(self);
	sz = current = self->pro->alloc_size;
	if(current>=size) return true;

	while(sz < size) sz += self->pro->realloc_threshold_length;

	new_data = realloc(self->pro->data, (sz*self->pro->item_size));
	if(NULL==new_data) return false;

	self->pro->data = new_data;
	self->pro->alloc_size = sz;
	fill_memory(self, current, sz);

	return true;
}

/**
 * @brief Default clone method
 *
 * @param dest clone dest data address
 * @param src clone source data address
 * @param len data item length
 *
 * @return true: succeed / false: failure
 */
static bool default_cloner_impl(void* dest, const void* const src, size_t len)
{
	assert(dest);
	assert(src);

	memcpy(dest, src, len);
	return true;
}


/**
 * @brief Get data address
 *
 * @param self Vector object
 * @param ofs data offset
 *
 * @return NULL: nothing / NOT NULL: data address
 */
static const void* const get_impl(Vector* self, size_t ofs)
{
	assert(self);
	if(self->pro->length <= ofs) return NULL;

	return &((uint8*)self->pro->data)[ofs * self->pro->item_size];
}


/**
 * @brief Set data from data address
 *
 * @param self Vector object
 * @param ofs data offset
 * @param data data address for set
 *
 * @return 
 */
static bool set_impl(Vector* self, size_t ofs, const void* const data)
{
	size_t new_size;
	uint8* ptr;

	assert(self);
	new_size = self->pro->length;
	if(new_size<=ofs) new_size = ofs+1;

	if(!alloc_impl(self,new_size)) return false;

	ptr = self->pro->data;
	memcpy(ptr+(self->pro->item_size*ofs), data, self->pro->item_size);
	self->pro->length = new_size;

	return true;
}


/**
 * @brief Get vector array length
 *
 * @param self Vector object
 *
 * @return array length
 */
static size_t length_impl(Vector* self)
{
	assert(self);
	return self->pro->length;
}


/**
 * @brief Get allocated memory length
 *
 * @param self Vector object
 *
 * @return allocated memory length ( ** not memory size ** )
 */
static size_t memlen_impl(Vector* self)
{
	assert(self);
	return self->pro->alloc_size;
}


/**
 * @brief Append data
 *
 * @param self Vector object
 * @param data data address
 * @param len data array length
 *
 * @return true: succeed / false: failure
 */
static bool append_impl(Vector* self, const void* const data, size_t len)
{
	size_t new_size;
	uint8* ptr;

	assert(self);
	new_size = self->pro->length+len;

	if(!alloc_impl(self,new_size)) return false;

	ptr = self->pro->data;
	memcpy(ptr+(self->pro->item_size*self->pro->length), data, self->pro->item_size*len);
	self->pro->length = new_size;

	return true;
}


/**
 * @brief Insert data into vector array
 *
 * @param self Vector object
 * @param ofs data offset
 * @param data data address
 * @param len data array length
 *
 * @return true: succeed / false: failure
 *
 * @return 
 */
static bool insert_impl(Vector* self, const size_t ofs, const void* const data, size_t len)
{
	size_t base;
	size_t new_size;
	size_t src,dst;
	uint8* ptr;

	assert(self);
	assert(len>=0);

	base = self->pro->length;
	if(ofs>base) base = ofs;
	new_size = base + len;

	if(!alloc_impl(self,new_size)) return false;

	ptr = self->pro->data;
	/* move data for insert */
	src = self->pro->item_size*ofs;
	dst = src+(self->pro->item_size*len);
	memcpy(ptr+dst, ptr+src, self->pro->item_size*len);
	/* insert data */
	memcpy(ptr+src, data, self->pro->item_size*len);
	self->pro->length = new_size;

	return true;
}


/**
 * @brief Remove data from vector array
 *
 * @param self Vector object
 * @param ofs data offset
 * @param len delete length
 *
 * @return deleted data numbers (0 == failure)
 */
static size_t remove_impl(Vector* self, const size_t ofs, const size_t len)
{
	size_t tail;
	size_t del_len;
	size_t new_len;
	size_t free_size;
	size_t shirk_size;
	size_t shirk_min;
	size_t fill_end;
	uint8  *src, *dst;

	assert(self);
	assert(len);
	if(self->pro->length <= ofs) return 0;

	del_len = len;
	tail = ofs + len + 1;
	if(tail > (self->pro->length))
	{
		del_len = self->pro->length-ofs;
	}
	if(0==del_len) return 0;

	cleanup_data(self, ofs, del_len);

	dst = (uint8*)self->pro->data + (self->pro->item_size * ofs);
	src = dst + (self->pro->item_size * del_len);
	memcpy(dst, src, ((self->pro->length-ofs-del_len) * self->pro->item_size));

	new_len = self->pro->length-del_len;
	free_size = self->pro->alloc_size - new_len;

	shirk_size = 0;
	shirk_min = (self->pro->realloc_threshold_length>>1) + self->pro->realloc_threshold_length;
	while(free_size >= shirk_min)
	{
		shirk_size += self->pro->realloc_threshold_length;
		free_size -= self->pro->realloc_threshold_length;
	}
	if(0!=shirk_size)
	{
		size_t new_size;
		void*  data;

		new_size = self->pro->alloc_size - shirk_size;
		if(new_size < self->pro->minimum_size) new_size = self->pro->minimum_size;

		data = realloc(self->pro->data, (new_size*self->pro->item_size));
		if(NULL==data) return 0;
		self->pro->data = data;
		self->pro->alloc_size = new_size;
	}

	fill_end = self->pro->length;
	if(self->pro->alloc_size < fill_end)
		fill_end = self->pro->alloc_size;
	fill_memory(self, new_len, fill_end);
	self->pro->length = new_len;
	return del_len;
}


/**
 * @brief Clean all vector data
 *
 * @param self Vector object
 *
 * @return true: succeed / false: failure
 */
static bool clear_impl(Vector* self)
{
	void* data;
	size_t sz;

	assert(self);

	if(0 == self->pro->length) return false;

	/* allocate memory */
	sz = self->pro->realloc_threshold_length;
	if(self->pro->minimum_size>self->pro->realloc_threshold_length)
		sz = self->pro->minimum_size;
	data = calloc(sz, self->pro->item_size);
	if(NULL==data) return false;

	cleanup_data(self, 0, self->pro->length);
	free(self->pro->data);

	/* reset parameters */
	self->pro->data = data;
	self->pro->alloc_size = sz;
	self->pro->length = 0;

	return true;
}


/**
 * @brief Connect vector data
 *
 * @param self Vector object
 * @param con Vector object for connect
 *
 * @return true: succeed / false: failure
 */
static bool connect_impl(Vector* self, const Vector* const con)
{
	assert(self);
	assert(con);

	/* type check */
	if(self->pro->item_size != con->pro->item_size) return false;

	/* connect */
	return self->append(self, con->pro->data, con->pro->length);
}


/**
 * @brief Get vector data's clone
 *
 * @param self Vector data
 *
 * @return NULL: failure / NOT NULL: Cloned vector data
 */
static Vector* clone_impl(const Vector* const self)
{
	Vector* cloned;
	size_t i;
	size_t item_size;

	assert(self);

	cloned = new_Vector_impl(self->pro->item_size, self->pro->realloc_threshold_length, self->pro->cloner, self->pro->deleter, self->pro->max_size, self->pro->minimum_size);
	cloned->alloc(cloned, self->pro->alloc_size);
	cloned->pro->length = self->pro->length;
	item_size = self->pro->item_size;
	for(i=0; i<self->pro->length; i++)
	{
		if(!self->pro->cloner((uint8*)(cloned->pro->data)+(i*item_size), (uint8*)(self->pro->data)+(i*item_size), item_size))
		{
			delete_Vector(&cloned);
			return NULL;
		}
	}

	return cloned;
}


