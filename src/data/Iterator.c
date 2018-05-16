/**
 * @file Iterator.c
 */
#include <assert.h>
#include <stdlib.h>
#include "data/Iterator.h"

/* this header isn't read from anything other */
/* than inherited object.                     */ 
#include "Iterator.protected.h"

/* prototypes */
static Iterator* prev_impl(Iterator*);
static Iterator* next_impl(Iterator*);
static void* data_impl(Iterator*);
static bool insert_impl(Iterator*, void*);
static void remove_impl(Iterator*, bool);


/*--------------- Constructor / Destructor ---------------*/

/**
 * @brief Create Iterator object
 *
 * @return the pointer of object
 */
Iterator* new_Iterator_impl(void* data)
{
	Iterator* self;
	Iterator_protected* pro;

	/* make objects */
	self = calloc(1, sizeof(Iterator));
	pro = calloc(1, sizeof(Iterator_protected));

	/* check whether object creatin succeeded */
	assert(pro);
	assert(self);

	/*--- set protected member ---*/
	pro->data = data;
	pro->prev = pro->next = NULL;

	/*--- set public member ---*/
	self->data = data_impl;
	self->prev = prev_impl;
	self->next = next_impl;
	self->insert = insert_impl;
	self->remove = remove_impl;

	/* init Iterator object */
	self->pro = pro;
	return self;
}

/**
 * @brief delete own member variables
 *
 * @param the pointer of object
 */
void delete_Iterator_members(Iterator* self)
{
	/* delete protected members */
	free(self->pro);
}

/**
 * @brief Delete Iterator object
 *
 * @param the pointer of object
 */
void delete_Iterator_impl(Iterator** self)
{
	/* This is the template that default destractor. */
	assert(self);
	if(NULL == (*self)) return;
	delete_Iterator_members(*self);
	free(*self);
	(*self) = NULL;
}


/*--------------- internal methods ---------------*/

/**
 * @brief Get previous iterator
 *
 * @param self Iterator object
 *
 * @return Previous iterator object
 */
static Iterator* prev_impl(Iterator* self)
{
	assert(self);
	return self->pro->prev;
}


/**
 * @brief Get next iterator
 *
 * @param self Iterator object
 *
 * @return Next iterator object
 */
static Iterator* next_impl(Iterator* self)
{
	assert(self);
	return self->pro->next;
}


/**
 * @brief Get data that iterator has
 *
 * @param self Iterator object
 *
 * @return Data pointer
 */
static void* data_impl(Iterator* self)
{
	assert(self);
	return self->pro->data;
}


/**
 * @brief Insert new data
 *
 * @param self Iterator object
 * @param data insert data
 *
 * @return true: succeed / false: failure
 */
static bool insert_impl(Iterator* self, void* data)
{
	Iterator *itPrev;
	Iterator *itNew;

	itNew = new_Iterator(data);
	assert(itNew);
	itPrev = self->prev(self);

	if(NULL != itPrev)
	{
		itPrev->pro->next = itNew;
	}

	itNew->pro->prev = self->pro->prev;
	itNew->pro->next = self;
	self->pro->prev = itNew;

	return true;
}


/**
 * @brief Remove iterator
 *
 * @param self Iterator object
 * @param isKeepData 
 */
static void remove_impl(Iterator* self, bool isKeepData)
{
	/* NOTE: Nothing to do currently. */
	return;
}
