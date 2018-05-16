/**
 * @file List.protected.h
 */
#ifndef LIST_PROTECTED_H
#define LIST_PROTECTED_H

/**
 * List main instance
 */
struct _List_protected {
	Iterator* top;
	Iterator* tail;
	size_t length;
	ListDataCloner_t cloner;
	ListDataDeleter_t deleter;
};

/**
 * detete own members method prototype
 * I recommend it that you move this method in "List.c"
 * if you plan to make the final class.
 */
void delete_List_members(List*);

#endif
