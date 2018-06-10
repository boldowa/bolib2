/**
 * @file Iterator.h
 */
#ifndef ITERATOR_H
#define ITERATOR_H

#include "btypes.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * public accessor
 */
typedef struct _Iterator Iterator;
typedef struct _Iterator_protected Iterator_protected;
typedef struct _Iterator_private Iterator_private;
struct _Iterator {
	Iterator* (*prev)(Iterator*);
	Iterator* (*next)(Iterator*);
	void* (*data)(Iterator*);
	bool  (*insert)(Iterator*, void*);
	void  (*remove)(Iterator*, bool);
	Iterator_protected* pro;
};

/**
 * Constructor
 */
Iterator* new_Iterator_impl(void*);
#define new_Iterator(a) new_Iterator_impl(a)

/**
 * Destractor
 */
void delete_Iterator_impl(Iterator**);
#define delete_Iterator(a) delete_Iterator_impl(a)


#ifdef __cplusplus
}
#endif
#endif
