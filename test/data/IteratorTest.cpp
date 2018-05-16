/**
 * @file IteratorTest.cpp
 */
#include <CppUTest/TestHarness.h>
#include "data/Iterator.h"

TEST_GROUP(IteratorTest)
{
	Iterator* target;
	int* itest;

	virtual void setup()
	{
		itest = (int*)malloc(sizeof(int));
		CHECK(NULL!=itest);
		*itest = 30;
		target = new_Iterator(itest);
	}

	virtual void teardown()
	{
		delete_Iterator(&target);
		free(itest);
	}
};


/**
 * Check object create
 */
TEST(IteratorTest, new)
{
	CHECK(NULL != target);

	/* Method */
	CHECK(NULL!=target->prev);
	CHECK(NULL!=target->next);
	CHECK(NULL!=target->data);
	CHECK(NULL!=target->insert);
	CHECK(NULL!=target->remove);

	/* protected member */
	CHECK(NULL!=target->pro);
}

/**
 * Check object delete
 */
TEST(IteratorTest, delete)
{
	delete_Iterator(&target);

	/* check delete */
	POINTERS_EQUAL(NULL, target);
}

/**
 * Check prev method
 */
TEST(IteratorTest, prev)
{
	POINTERS_EQUAL(NULL, target->prev(target));
}

/**
 * Check next method
 */
TEST(IteratorTest, next)
{
	POINTERS_EQUAL(NULL, target->next(target));
}

/**
 * Check data method
 */
TEST(IteratorTest, data)
{
	int *i;
	i = (int*)target->data(target);
	CHECK(NULL != i);
	LONGS_EQUAL(30, *i);
}

/**
 * Check insert method
 */
TEST(IteratorTest, insert)
{
	int *ins;
	int *chk;
	Iterator* prev;
	Iterator* ppre;
	
	ins = (int*)malloc(sizeof(int));
	CHECK(NULL!=ins);
	*ins = 100;

	target->insert(target, ins);
	prev = target->prev(target);

	CHECK(NULL != prev);
	chk = (int*)prev->data(prev);
	LONGS_EQUAL(100, (*chk));

	
	ins = (int*)malloc(sizeof(int));
	CHECK(NULL!=ins);
	*ins = 999;

	target->insert(target, ins);
	ppre = target->prev(target);

	CHECK(NULL != ppre);
	chk = (int*)ppre->data(ppre);
	LONGS_EQUAL(999, (*chk));

	free(prev->data(prev));
	free(ppre->data(ppre));
	delete_Iterator(&prev);
	delete_Iterator(&ppre);
}

/**
 * Check remove method
 */
TEST(IteratorTest, remove)
{
	/* TODO: implement this test if implement remove method */
	target->remove(target, true);
}

