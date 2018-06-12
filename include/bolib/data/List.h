/**
 * @file List.h
 */
#ifndef LIST_H
#define LIST_H

#include "bolib/btypes.h"
#include "bolib/data/Iterator.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef void* (*ListDataCloner_t)(const void*);
typedef void (*ListDataDeleter_t)(void*);
typedef bool (*ListSearcher_t)(const void*, const void*);

/**
 * public accessor
 */
typedef struct _List List;
typedef struct _List_protected List_protected;
typedef struct _List_private List_private;
struct _List {
	size_t (*length)(List*);
	bool (*enqueue)(List*, void* data);
	bool (*push)(List*, void*);
	bool (*pushBack)(List*, void*);
	bool (*pushFront)(List*, void*);
	void* (*dequeue)(List*);
	void* (*pop)(List*);
	void* (*popBack)(List*);
	void* (*popFront)(List*);
	void* (*index)(List*, const size_t);
	Iterator* (*begin)(List*);
	Iterator* (*end)(List*);
	Iterator* (*search)(List*, const void*, ListSearcher_t);
	Iterator* (*searchex)(List* self, const void* data, ListSearcher_t match, Iterator* beg, const bool isPrevSearch);
	List* (*clone)(List*);
	bool (*remove)(List*, Iterator*);
	/* protected members */
	List_protected* pro;
};

/**
 * Constructor
 */
List* new_List_impl(void*(*)(const void* const), void(*)(void*));
#define new_List(a,b) new_List_impl(a,b)

/**
 * Destractor
 */
void delete_List_impl(List**);
#define delete_List(a) delete_List_impl(a)


#ifdef __cplusplus
}
#endif
#endif
