/**
 * @file Iterator.protected.h
 */
#ifndef ITERATOR_PROTECTED_H
#define ITERATOR_PROTECTED_H

/**
 * Iterator main instance
 */
struct _Iterator_protected {
	Iterator *prev;
	Iterator *next;
	void * data;
};

/**
 * detete own members method prototype
 * I recommend it that you move this method in "Iterator.c"
 * if you plan to make the final class.
 */
void delete_Iterator_members(Iterator*);

#endif
