#include "datastructures/unit/hashtable.unit.h"

#include "datastructures/hashtable.h"

#include <stdlib.h>

// hashtable* create_hashtable(size_t capacity, hasher h, comparator c)
IMPLEMENT_TEST(create_hashtable)
{
    hashtable* table = create_hashtable(0, hasher_ptr, comparator_ptr);
    if (table)
    {
        free_hashtable(table);
        FAIL_TEST("Created a hash table with a capacity of 0");
    }

    table = create_hashtable(1, NULL, comparator_ptr);
    if (table)
    {
        free_hashtable(table);
        FAIL_TEST("Created a hash table without a hash functon");
    }

    table = create_hashtable(1, hasher_ptr, NULL);
    if (table)
    {
        free_hashtable(table);
        FAIL_TEST("Created a hash table without a compare functon");
    }

    table = create_hashtable(1, hasher_ptr, comparator_ptr);
    if (!table)
    {
        FAIL_TEST("Could not create a hashtable with the required arguments");
    }

    free_hashtable(table);

    PASS_TEST();
}

uint64_t hasher_uint64_t_noop(void* key)
{
    return *((uint64_t*) key);
}

// void* get_hashtable(hashtable* table, void* key)
// bool set_hashtable(hashtable* table, void* key, void* value)
IMPLEMENT_TEST(setGet_hashtable)
{
    hashtable* table = create_hashtable(5, hasher_uint64_t_noop, comparator_uint64_t);
    if (!table)
    {
        FAIL_TEST("Could not create a hashtable with the required arguments");
    }

    uint64_t key1 = 0;
    char val1[] = "Value 1";

    void* retreiver;

    if (!set_hashtable(table, &key1, val1))
    {
        free_hashtable(table);
        FAIL_TEST("Could not set hashtable with key1 and val1");
    }

    retreiver = get_hashtable(table, &key1);
    if (retreiver != val1)
    {
        free_hashtable(table);
        FAIL_TEST("Could not get hastable with key1");
    }

    uint64_t key2 = 1;
    char val2[] = "Value 2";

    if (!set_hashtable(table, &key2, val2))
    {
        free_hashtable(table);
        FAIL_TEST("Could not set hashtable with key2 and val2");
    }

    retreiver = get_hashtable(table, &key2);
    if (retreiver != val2)
    {
        free_hashtable(table);
        FAIL_TEST("Could not get hastable with key2");
    }

    uint64_t key3 = 10;
    char val3[] = "Value 3";

    if (!set_hashtable(table, &key3, val3))
    {
        free_hashtable(table);
        FAIL_TEST("Could not set hashtable with key3 with hash collision with key1");
    }

    retreiver = get_hashtable(table, &key3);
    if (retreiver != val3)
    {
        free_hashtable(table);
        FAIL_TEST("Could not get hastable with key3 with hash collision with key1");
    }

    retreiver = get_hashtable(table, &key1);
    if (retreiver != val1)
    {
        free_hashtable(table);
        FAIL_TEST("Could not get hastable with key1 the second time");
    }

    retreiver = get_hashtable(table, &key2);
    if (retreiver != val2)
    {
        free_hashtable(table);
        FAIL_TEST("Could not get hastable with key2 the second time");
    }

    void** allVals = getAll_hashtable(table);
    if (!allVals)
    {
        FAIL_TEST("getAll_hashtable returned NULL");
    }
    else if (allVals[0] != &val1 || allVals[1] != &val2 || allVals[2] != &val3)
    {
        free(allVals);
        free_hashtable(table);

        FAIL_TEST("All values not returned");
    }

    free_hashtable(table);

    PASS_TEST();
}
