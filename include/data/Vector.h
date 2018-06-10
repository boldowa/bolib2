/**
 * @file Vector.h
 */
#ifndef VECTOR_H
#define VECTOR_H

#include "btypes.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef bool (*VectorDataCloner_t)(void*, const void* const, size_t);
typedef void (*VectorDataDeleter_t)(void*);

/**
 * public accessor
 */
typedef struct _Vector Vector;
typedef struct _Vector_protected Vector_protected;
typedef struct _Vector_private Vector_private;
struct _Vector {
	/* member method */
	const void* const (*get)(Vector*, size_t);
	bool (*set)(Vector*, size_t, const void* const);
	size_t (*length)(Vector*);
	size_t (*memlen)(Vector*);
	bool (*append)(Vector*, const void* const, const size_t);
	bool (*insert)(Vector*, const size_t, const void* const, const size_t);
	size_t (*remove)(Vector*, const size_t, const size_t);
	bool (*clear)(Vector*);
	bool (*connect)(Vector*, const Vector* const);
	bool (*alloc)(Vector*, size_t);
	Vector* (*clone)(const Vector* const);
	/* protected members */
	Vector_protected* pro;
};

/**
 * Constructor
 */
Vector* new_Vector_impl(size_t item_size, size_t threshold, VectorDataCloner_t cloner, VectorDataDeleter_t deleter, size_t max, size_t minimum);
#define new_Vector(type, threshold, cloner, deleter) new_Vector_impl(sizeof(type), threshold, cloner, deleter, 0, threshold)
#define new_Vector_with_max(type, threshold, cloner, deleter, max) new_Vector_impl(sizeof(type), threshold, cloner, deleter, max, threshold)
#define new_Vector_with_maxmin(type, threshold, cloner, deleter, max, min) new_Vector_impl(sizeof(type), threshold, cloner, deleter, max, min)

/**
 * Destractor
 */
void delete_Vector_impl(Vector**);
#define delete_Vector(a) delete_Vector_impl(a)


#ifdef __cplusplus
}
#endif
#endif
