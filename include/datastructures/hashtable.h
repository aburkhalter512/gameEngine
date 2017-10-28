#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef uint64_t (*hasher)(void*);
typedef bool (*comparator)(void*, void*);

typedef struct _hashtable hashtable;

/*
Computes the hash of a uint64_t
*/
uint64_t hasher_uint64_t(void* ptr);

/*
Dereferences the pointers to uint64_t and compares them. True is returned if they
are equal
*/
bool comparator_uint64_t(void* p1, void* p2);

/*
Computes the hash from the address of a pointer
*/
uint64_t hasher_ptr(void* ptr);

/*
Returns true if two pointer addresses are equal
*/
bool comparator_ptr(void* p1, void* p2);

/*
Computes the hash of a NULL terminated c-string
*/
uint64_t hasher_string(void* ptr);

/*
Returns true if the two NULL terminiated c-strings are equal.
*/
bool comparator_string(void* p1, void* p2);

/*
Creates a new hashtable with a given capacity

Arguments
    size_t capacity: The initial capacity of hashtable.

    hasher h: The hash function to use when accessing elements in the hash table

    comparator c: The comparator function to use to see if two keys match exactly
*/
hashtable* create_hashtable(size_t capacity, hasher h, comparator c);

/*
Frees memory associated with the given hash table. This does not free the individual
keys or values inserted into this hash table.

Arguments
    hashtable* table: The hash table to free

Returns
    Returns true if the hash table's memory was successfully freed, false if table == NULL
*/
bool free_hashtable(hashtable* table);

/*
Gets a value from a hash table by a key

Arguments
    hashtable* table: The hashtable to retrieve the value from

    void* key: The key of the value to retrieve

Returns
    Returns the value for the key, or NULL if the key does not exist
*/
void* get_hashtable(hashtable* table, void* key);

/*
Inserts a new value into the hash table associated with a given key.

Arguments
    hashtable* table: The hashtable to insert the new value into

    void* key: The key of the value to insert

    void* value: The value to insert

Precondition
    key has not been inserted into this hash table previous. All keys should be unique

Returns
    Returns true if the value was successfully inserted, false if any of the arguments are NULL
    or if memory allocation failed
*/
bool set_hashtable(hashtable* table, void* key, void* value);

/*
Returns an array of all of the values inserted into the hash table. No order is guaranteed

Arguments
    hashtable* table: The hash table to retrieve all of the values from

Returns
    Returns an array of the values inserted into the hash table. This memory must
    be deallocated when finished.
*/
void** getAll_hashtable(hashtable* table);

/*
Returns the number of values inserted in the hash table

Arguments
    hashtable* table: The table to get the number of values from

Returns
    Returns the number of values contained in this hash table
*/
size_t getCount_hashtable(hashtable* table);

/*
Clears all key/values from the hashtable.

Arguments
    hashtable* table: The table to clear
*/
void clear_hashtable(hashtable* table);
