/**
 * @file Vector.protected.h
 */
#ifndef VECTOR_PROTECTED_H
#define VECTOR_PROTECTED_H

/**
 * Vector main instance
 */
struct _Vector_protected {
	void*  data;
	size_t alloc_size;
	size_t length;
	size_t item_size;
	size_t minimum_size;
	size_t max_size;
	size_t realloc_threshold_length;
	VectorDataCloner_t cloner;
	VectorDataDeleter_t deleter;
};

/**
 * detete own members method prototype
 * I recommend it that you move this method in "Vector.c"
 * if you plan to make the final class.
 */
void delete_Vector_members(Vector*);

#endif
