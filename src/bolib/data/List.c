/**
 * @file List.c
 */
#include <assert.h>
#include <stdlib.h>
#include "bolib/data/List.h"

/* this header isn't read from anything other */
/* than inherited object.                     */ 
#include "List.protected.h"
#include "Iterator.protected.h"

/* prototypes */
static size_t length_impl(List*);
static bool pushBack_impl(List*, void*);
static bool pushFront_impl(List*, void*);
static void* popBack_impl(List*);
static void* popFront_impl(List*);
static void* index_impl(List*, const size_t);
static Iterator* begin_impl(List*);
static Iterator* end_impl(List*);
static Iterator* search_impl(List*, const void*, ListSearcher_t);
static Iterator* searchex_impl(List* self, const void* data, ListSearcher_t match, Iterator* beg, const bool isPrevSearch);
static List* clone_impl(List*);
static bool remove_impl(List*, Iterator*);


/*--------------- Constructor / Destructor ---------------*/

/**
 * @brief Create the List object
 *
 * @param cloner data clone function pointer
 * @param deleter data delete function pointer
 *
 * @return The List object
 */
List* new_List_impl(void*(*cloner)(const void* const), void(*deleter)(void*))
{
	List* self;
	List_protected* pro;

	/* make objects */
	self = calloc(1,sizeof(List));
	pro = calloc(1,sizeof(List_protected));

	/* check whether object creatin succeeded */
	assert(pro);
	assert(self);

	/*--- set protected member ---*/
	/* TODO : Fix it */
	pro->cloner = cloner;
	pro->deleter = deleter;
	pro->length = 0;
	pro->top = NULL;
	pro->tail = NULL;

	/*--- set public member ---*/
	/*--- push ---*/
	self->push = pushBack_impl;
	self->pushFront = pushFront_impl;
	self->pushBack = pushBack_impl;
	self->enqueue = pushBack_impl;
	/*--- pop ---*/
	self->pop = popBack_impl;
	self->popFront = popFront_impl;
	self->popBack = popBack_impl;
	self->dequeue = popFront_impl;
	/*--- util ---*/
	self->length = length_impl;
	self->index = index_impl;
	self->begin = begin_impl;
	self->end = end_impl;
	self->search = search_impl;
	self->searchex = searchex_impl;
	self->clone = clone_impl;
	self->remove = remove_impl;

	/* init List object */
	self->pro = pro;
	return self;
}

/**
 * @brief delete own member variables
 *
 * @param the pointer of object
 */
void delete_List_members(List* self)
{
	List_protected* lp;
	Iterator* it;
	Iterator* nx;
	/* delete protected members */
	lp = self->pro;
	it = lp->top;
	while(NULL != it)
	{
		nx = it->next(it);
		lp->deleter(it->data(it));
		delete_Iterator(&it);
		it = nx;
	}

	/* delete protected members */
	free(self->pro);
}

/**
 * @brief Delete List object
 *
 * @param the pointer of object
 */
void delete_List_impl(List** self)
{
	/* This is the template that default destractor. */
	assert(self);
	if(NULL == (*self)) return;
	delete_List_members(*self);
	free(*self);
	(*self) = NULL;
}


/*--------------- internal methods ---------------*/

/**
 * @brief Get list length
 *
 * @param self List object
 *
 * @return Length of list
 */
static size_t length_impl(List* self)
{
	assert(self);
	return self->pro->length;
}


/**
 * @brief Push data from front
 *
 * @param self List object
 * @param data data pointer
 *
 * @return true: succeed / false: failure
 */
static bool pushFront_impl(List* self, void* data)
{
	Iterator* it;
	Iterator_protected* itp;
	List_protected* sep;

	assert(self);
	it = new_Iterator(data);
	itp = it->pro;
	sep = self->pro;

	itp->next = sep->top;
	if(NULL != sep->top)
	{
		sep->top->pro->prev = it;
	}
	sep->top = it;
	if(0 == sep->length)
	{
		sep->tail = sep->top;
	}
	sep->length++;
	return true;
}


/**
 * @brief Push data from back
 *
 * @param self List object
 * @param data data pointer
 *
 * @return true: succeed / false: failure
 */
static bool pushBack_impl(List* self, void* data)
{
	Iterator* it;
	Iterator_protected* itp;
	List_protected* sep;

	assert(self);
	it = new_Iterator(data);
	itp = it->pro;
	sep = self->pro;

	itp->prev = sep->tail;
	if(NULL != sep->tail)
	{
		sep->tail->pro->next = it;
	}
	sep->tail = it;
	if(0 == sep->length)
	{
		sep->top = sep->tail;
	}
	sep->length++;
	return true;
}


/**
 * @brief Pop data from front
 *
 * @param self List object
 *
 * @return data pointer
 */
static void* popFront_impl(List* self)
{
	Iterator* it;
	Iterator_protected* itp;
	List_protected* sep;
	void* data;

	assert(self);
	sep = self->pro;
	if(NULL == sep->top)
	{
		return NULL;
	}

	/* get data */
	itp = sep->top->pro;
	data = itp->data;

	/* delete iterator */
	it = sep->top->pro->next;
	delete_Iterator(&sep->top);

	/* list update */
	if(NULL != it)
	{
		it->pro->prev = NULL;
	}
	sep->top = it;
	sep->length--;
	if(0 == sep->length)
	{
		sep->tail = sep->top;
	}

	return data;
}


/**
 * @brief Pop data from back
 *
 * @param self List object
 *
 * @return data pointer
 */
static void* popBack_impl(List* self)
{
	Iterator* it;
	Iterator_protected* itp;
	List_protected* sep;
	void* data;

	assert(self);
	sep = self->pro;
	if(NULL == sep->tail)
	{
		return NULL;
	}

	/* get data */
	itp = sep->tail->pro;
	data = itp->data;

	/* delete iterator */
	it = sep->tail->pro->prev;
	delete_Iterator(&sep->tail);

	/* list update */
	if(NULL != it)
	{
		it->pro->next = NULL;
	}
	sep->tail = it;
	sep->length--;
	if(0 == sep->length)
	{
		sep->top = sep->tail;
	}

	return data;
}


/**
 * @brief Get data
 *
 * @param self List object
 * @param inx data index
 *
 * @return data pointer
 */
static void* index_impl(List* self, const size_t inx)
{
	Iterator_protected* itp;
	List_protected* sep;
	size_t i;

	assert(self);

	/* valid value check */
	/*if(0 > inx) return NULL;*/
	if(self->pro->length <= inx) return NULL;

	/* list empty check */
	sep = self->pro;
	if(NULL == sep->top) return NULL;

	sep = self->pro;

	/* seek from top */
	if(sep->length/2 > inx)
	{
		itp = sep->top->pro;
		for(i = 0; i != inx; i++)
		{
			itp = itp->next->pro; 
		}
		return itp->data;
	}

	/* seek from tail */
	itp = sep->tail->pro;
	for(i = sep->length-1; i != inx; i--)
	{
		itp = itp->prev->pro;
	}
	return itp->data;
}


/**
 * @brief Get top data iterator
 *
 * @param self List object
 *
 * @return Iterator object
 */
static Iterator* begin_impl(List* self)
{
	assert(self);
	return self->pro->top;
}


/**
 * @brief Get tail data iterator
 *
 * @param self List object
 *
 * @return Iterator object
 */
static Iterator* end_impl(List* self)
{
	assert(self);
	return self->pro->tail;
}


/**
 * @brief Search data from list
 *
 * @param self List object
 * @param data target data
 * @param match match function pointer
 * @param beg Iterator object that starts search
 * @param isPrevSearch true: previous / false: incremental
 *
 * @return NULL: Not found / NOT NULL: Iterator object that found data
 */
static Iterator* searchex_impl(List* self, const void* data, ListSearcher_t match, Iterator* beg, const bool isPrevSearch)
{
	Iterator* it;
	Iterator_protected* itp;
	assert(self);

	if(NULL == beg || NULL == data || NULL == match)
	{
		return NULL;
	}
	it = beg;

	if(false == isPrevSearch)
	{
		while(NULL != it)
		{
			itp = it->pro;
			if(true == match(data, itp->data))
			{
				return it;
			}
			it = itp->next;
		}
		return NULL;
	}

	while(NULL != it)
	{
		itp = it->pro;
		if(true == match(data, itp->data))
		{
			return it;
		}
		it = itp->prev;
	}
	return NULL;
}


/**
 * @brief Search data from list
 *
 * @param self List object
 * @param data target data
 * @param match match function pointer
 *
 * @return NULL: Not found / NOT NULL: Iterator object that found data
 */
static Iterator* search_impl(List* self, const void* data, ListSearcher_t match)
{
	/* search from top */
	return searchex_impl(self, data, match, self->pro->top, false);
}


/**
 * @brief Clone List object
 *
 * @param src List object
 *
 * @return List object that cloned
 */
static List* clone_impl(List* src)
{
	List* dest;
	List_protected* srp;
	Iterator* sit;
	void* data;

	assert(src);

	srp = src->pro;
	sit = srp->top;
	dest = new_List(srp->cloner, srp->deleter);
	assert(dest);

	while(sit != NULL)
	{
		data = srp->cloner(sit->data(sit));
		assert(data);
		dest->push(dest, data);
		sit = sit->next(sit);
	}

	return dest;
}


/**
 * @brief Remove item from List
 *
 * @param self List object
 * @param item Iterator object that removes
 *
 * @return true: succeed / false: failure
 */
static bool remove_impl(List* self, Iterator* item)
{
	Iterator* it;
	void* d;
	if(NULL == self->pro->top) return false;

	/* match top */
	if(item == self->pro->top)
	{
		d = self->popFront(self);
		self->pro->deleter(d);
		return true;
	}

	/* match tail */
	if(item == self->pro->tail)
	{
		d = self->pop(self);
		self->pro->deleter(d);
		return true;
	}

	/* search */
	it = self->pro->top->pro->next;
	while(NULL != it)
	{
		if(it == item)
		{
			/* prev pointer change */
			it = item->pro->prev;
			it->pro->next = item->pro->next;

			/* next pointer change */
			it = item->pro->next;
			it->pro->prev = item->pro->prev;

			/* delete */
			self->pro->deleter(item->pro->data);
			delete_Iterator(&item);

			self->pro->length--;

			return true;
		}
		it = it->next(it);
	}

	/* not found */
	return false;
}
