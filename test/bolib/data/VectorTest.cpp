/**
 * @file VectorTest.cpp
 */
#include <CppUTest/TestHarness.h>
#include "bolib/data/Vector.h"

static const size_t Threshold = 0x200;

TEST_GROUP(Vector)
{
	Vector* target;
	virtual void setup()
	{
		target = new_Vector(int,Threshold,NULL,NULL);
	}

	virtual void teardown()
	{
		delete_Vector(&target);
	}
};


/**
 * Check object create
 */
TEST(Vector, new)
{
	CHECK(NULL != target);

	/* Method and protected */
	CHECK(NULL!=target->get);
	CHECK(NULL!=target->set);
	CHECK(NULL!=target->length);
	CHECK(NULL!=target->memlen);
	CHECK(NULL!=target->append);
	CHECK(NULL!=target->insert);
	CHECK(NULL!=target->remove);
	CHECK(NULL!=target->clear);
	CHECK(NULL!=target->connect);
	CHECK(NULL!=target->alloc);
	CHECK(NULL!=target->clone);
	CHECK(NULL!=target->pro);

	/* initial check */
	LONGS_EQUAL(0, target->length(target));
	LONGS_EQUAL(Threshold, target->memlen(target));
	POINTERS_EQUAL(NULL, target->get(target, 0));
}


/**
 * Check object delete
 */
TEST(Vector, delete)
{
	delete_Vector(&target);

	/* check delete */
	POINTERS_EQUAL(NULL, target);
}


/**
 * Check get method
 */
TEST(Vector, get)
{
	/* Nothing to do here.        */
	/* (Check in set method test) */
}


/**
 * Check set method
 */
TEST(Vector, set)
{
	int value;
	int value2;
	int value3;
	int result;
	int* p;

	/* set */
	value=12345;
	CHECK(target->set(target, 0, &value));
	LONGS_EQUAL(1, target->length(target));
	LONGS_EQUAL(Threshold, target->memlen(target));
	p = (int*)target->get(target, 0);
	CHECK(NULL!=p);
	result = (*p);
	LONGS_EQUAL(value, result);


	/* automatic length expansion & set */
	value2=45645;
	CHECK(target->set(target, 4, &value2));
	LONGS_EQUAL(5, target->length(target));
	LONGS_EQUAL(Threshold, target->memlen(target));
	p = (int*)target->get(target, 4);
	CHECK(NULL!=p);
	result = (*p);
	LONGS_EQUAL(value2, result);
	p = (int*)target->get(target, 0);
	CHECK(NULL!=p);
	result = (*p);
	LONGS_EQUAL(value, result);


	/* automatic memory allocation & set */
	value3 = 364364;
	CHECK(target->set(target, Threshold+3, &value3));
	LONGS_EQUAL(Threshold+4, target->length(target));
	LONGS_EQUAL(Threshold*2, target->memlen(target));
	p = (int*)target->get(target, Threshold+3);
	CHECK(NULL!=p);
	result = (*p);
	LONGS_EQUAL(value3, result);
	p = (int*)target->get(target, 4);
	CHECK(NULL!=p);
	result = (*p);
	LONGS_EQUAL(value2, result);
	p = (int*)target->get(target, 0);
	CHECK(NULL!=p);
	result = (*p);
	LONGS_EQUAL(value, result);
}


/**
 * Check length method
 */
TEST(Vector, length)
{
	/* nothing to do here. */
}


/**
 * Check memlen method
 */
TEST(Vector, memlen)
{
	/* nothing to do here. */
}


/**
 * Check append method
 */
TEST(Vector, append)
{
	int check[4] = {111,234,555,194};
	int check2[2] = {555, 356};
	int check3[3] = {345, 678, 1919};
	int* p;

	/* append */
	CHECK(target->append(target, check, 4));
	LONGS_EQUAL(4, target->length(target));
	LONGS_EQUAL(Threshold, target->memlen(target));
	p = (int*)target->get(target, 0);
	CHECK(NULL!=p);
	LONGS_EQUAL(check[0], p[0]);
	LONGS_EQUAL(check[1], p[1]);
	LONGS_EQUAL(check[2], p[2]);
	LONGS_EQUAL(check[3], p[3]);

	/* append */
	CHECK(target->append(target, check2, 2));
	LONGS_EQUAL(6, target->length(target));
	LONGS_EQUAL(Threshold, target->memlen(target));
	p = (int*)target->get(target, 0);
	CHECK(NULL!=p);
	LONGS_EQUAL(check[0], p[0]);
	LONGS_EQUAL(check[1], p[1]);
	LONGS_EQUAL(check[2], p[2]);
	LONGS_EQUAL(check[3], p[3]);
	LONGS_EQUAL(check2[0], p[4]);
	LONGS_EQUAL(check2[1], p[5]);

	/* auto expand append */
	CHECK(target->set(target,Threshold-1, check)); /* add dummy data at tail */
	CHECK(target->append(target, check3, 3));
	LONGS_EQUAL(Threshold+3, target->length(target));
	LONGS_EQUAL(Threshold*2, target->memlen(target));
	p = (int*)target->get(target, Threshold);
	CHECK(NULL!=p);
	LONGS_EQUAL(check3[0], p[0]);
	LONGS_EQUAL(check3[1], p[1]);
	LONGS_EQUAL(check3[2], p[2]);
}


/**
 * Check insert method
 */
TEST(Vector, insert)
{
	int check[5] = {123,555,989,184,3};
	int* p;

	/* insert top */
	CHECK(target->insert(target,0,&check[0],1));
	LONGS_EQUAL(1, target->length(target));
	LONGS_EQUAL(Threshold, target->memlen(target));
	p = (int*)target->get(target, 0);
	CHECK(NULL!=p);
	LONGS_EQUAL(check[0], p[0]);

	/* insert tail */
	CHECK(target->insert(target,target->length(target),&check[3],2));
	LONGS_EQUAL(3, target->length(target));
	LONGS_EQUAL(Threshold, target->memlen(target));
	p = (int*)target->get(target, 0);
	CHECK(NULL!=p);
	LONGS_EQUAL(check[0], p[0]);
	LONGS_EQUAL(check[3], p[1]);
	LONGS_EQUAL(check[4], p[2]);

	/* insert mid */
	CHECK(target->insert(target,1,&check[1],2));
	LONGS_EQUAL(5, target->length(target));
	LONGS_EQUAL(Threshold, target->memlen(target));
	p = (int*)target->get(target, 0);
	CHECK(NULL!=p);
	LONGS_EQUAL(check[0], p[0]);
	LONGS_EQUAL(check[1], p[1]);
	LONGS_EQUAL(check[2], p[2]);
	LONGS_EQUAL(check[3], p[3]);
	LONGS_EQUAL(check[4], p[4]);

	/* auto expand */
	CHECK(target->insert(target,Threshold,check,5));
	LONGS_EQUAL(Threshold+5, target->length(target));
	LONGS_EQUAL(Threshold*2, target->memlen(target));
	p = (int*)target->get(target, Threshold);
	CHECK(NULL!=p);
	LONGS_EQUAL(check[0], p[0]);
	LONGS_EQUAL(check[1], p[1]);
	LONGS_EQUAL(check[2], p[2]);
	LONGS_EQUAL(check[3], p[3]);
	LONGS_EQUAL(check[4], p[4]);
}


/**
 * Check remove method
 */
TEST(Vector, remove1)
{
	int check[5] = {999,7688,114514810,8,42};
	int* p;

	/* null remove */
	LONGS_EQUAL(0, target->remove(target,0,1));

	/* prepare data */
	CHECK(target->append(target,check,5));
	LONGS_EQUAL(5, target->length(target));
	LONGS_EQUAL(Threshold, target->memlen(target));
	p = (int*)target->get(target, 0);
	CHECK(NULL!=p);
	LONGS_EQUAL(check[0], p[0]);
	LONGS_EQUAL(check[1], p[1]);
	LONGS_EQUAL(check[2], p[2]);
	LONGS_EQUAL(check[3], p[3]);
	LONGS_EQUAL(check[4], p[4]);

	/* remove out of vector */
	LONGS_EQUAL(0, target->remove(target,5,1));
	LONGS_EQUAL(0, target->remove(target,Threshold,1));

	/* remove tail data */
	LONGS_EQUAL(1, target->remove(target,4,1));
	LONGS_EQUAL(4, target->length(target));
	LONGS_EQUAL(Threshold, target->memlen(target));

	/* remove top data */
	LONGS_EQUAL(1, target->remove(target,0,1));
	LONGS_EQUAL(3, target->length(target));
	LONGS_EQUAL(Threshold, target->memlen(target));
	p = (int*)target->get(target, 0);
	CHECK(NULL!=p);
	LONGS_EQUAL(check[1], p[0]);
	LONGS_EQUAL(check[2], p[1]);
	LONGS_EQUAL(check[3], p[2]);

	/* remove mid data */
	LONGS_EQUAL(1, target->remove(target,1,1));
	LONGS_EQUAL(2, target->length(target));
	LONGS_EQUAL(Threshold, target->memlen(target));
	p = (int*)target->get(target, 0);
	CHECK(NULL!=p);
	LONGS_EQUAL(check[1], p[0]);
	LONGS_EQUAL(check[3], p[1]);

	/* remove size over */
	LONGS_EQUAL(2, target->remove(target,0,100));
	LONGS_EQUAL(0, target->length(target));
	LONGS_EQUAL(Threshold, target->memlen(target));
}

TEST(Vector, remove2)
{
	int check[5] = {999,7688,114514810,8,42};
	int* p;

	/* prepare data */
	CHECK(target->insert(target,Threshold*5,check,5));
	LONGS_EQUAL(Threshold*5+5, target->length(target));
	LONGS_EQUAL(Threshold*6, target->memlen(target));
	p = (int*)target->get(target, Threshold*5);
	CHECK(NULL!=p);
	LONGS_EQUAL(check[0], p[0]);
	LONGS_EQUAL(check[1], p[1]);
	LONGS_EQUAL(check[2], p[2]);
	LONGS_EQUAL(check[3], p[3]);
	LONGS_EQUAL(check[4], p[4]);

	/* auto shirk(shirk-less) */
	LONGS_EQUAL(6, target->remove(target,target->length(target)-7,6));
	LONGS_EQUAL(Threshold*5-1, target->length(target));
	LONGS_EQUAL(Threshold*6, target->memlen(target));

	/* auto shirk */
	LONGS_EQUAL(Threshold*4+Threshold/2, target->remove(target,0,Threshold*4+Threshold/2));
	LONGS_EQUAL((Threshold/2)-1, target->length(target));
	LONGS_EQUAL(Threshold, target->memlen(target));
}


/**
 * Check clear method
 */
TEST(Vector, clear)
{
	int check = 444;

	/* prepare */
	CHECK(target->set(target,Threshold*2,&check));
	LONGS_EQUAL(Threshold*2+1, target->length(target));
	LONGS_EQUAL(Threshold*3, target->memlen(target));

	/* clear check */
	CHECK(target->clear(target));
	LONGS_EQUAL(0, target->length(target));
	LONGS_EQUAL(Threshold, target->memlen(target));

	/* clear-less */
	CHECK_FALSE(target->clear(target));
}


/**
 * Check connect method
 */
TEST(Vector, connect)
{
	Vector* con;
	int check1[4] = {111,234,555,194};
	int check2[2] = {555, 356};
	int *p;

	con = new_Vector(int, Threshold, NULL, NULL);

	/* make test data */
	target->append(target, check1, 4);
	con->append(con, check2, 2);
	LONGS_EQUAL(4, target->length(target));
	LONGS_EQUAL(2, con->length(con));

	/* connect */
	CHECK(target->connect(target, con));
	LONGS_EQUAL(6, target->length(target));
	p = (int*)target->get(target, 0);
	CHECK(NULL!=p);
	LONGS_EQUAL(check1[0], p[0]);
	LONGS_EQUAL(check1[1], p[1]);
	LONGS_EQUAL(check1[2], p[2]);
	LONGS_EQUAL(check1[3], p[3]);
	LONGS_EQUAL(check2[0], p[4]);
	LONGS_EQUAL(check2[1], p[5]);

	delete_Vector(&con);
}


/**
 * Check alloc method
 */
TEST(Vector, alloc)
{
	size_t mlen;
	const int Alloced=0x1300;

	mlen = target->memlen(target);
	CHECK(mlen<Alloced);

	CHECK(target->alloc(target,Alloced));
	CHECK(mlen<=target->memlen(target));
	CHECK(Alloced<=target->memlen(target));
}


/**
 * Check clone method
 */
TEST(Vector, clone)
{
	Vector* cloned;
	int check1[4] = {111,234,555,194};
	int *p1;
	int *p2;

	target->append(target, check1, 4);
	target->alloc(target, 0x1000);

	cloned = target->clone(target);

	/* check cloned */
	LONGS_EQUAL(cloned->memlen(cloned), target->memlen(target));
	LONGS_EQUAL(cloned->length(cloned), target->length(target));
	p1 = (int*)cloned->get(cloned, 0);
	CHECK(NULL!=p1);
	p2 = (int*)target->get(target, 0);
	CHECK(NULL!=p2);
	CHECK(p1 != p2);
	LONGS_EQUAL(p1[0], p2[0]);
	LONGS_EQUAL(p1[1], p2[1]);
	LONGS_EQUAL(p1[2], p2[2]);
	LONGS_EQUAL(p1[3], p2[3]);

	/* modify original data */
	CHECK(target->set(target,0,&check1[3]));
	CHECK(p1[0] != p2[0]);
	CHECK(target->clear(target));
	CHECK(cloned->memlen(cloned) != target->memlen(target));
	CHECK(cloned->length(cloned) != target->length(target));

	delete_Vector(&cloned);
}


