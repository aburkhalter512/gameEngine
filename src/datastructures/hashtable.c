#include "datastructures/hashtable.h"

#include <stdlib.h>
#include <string.h>

typedef struct _bucketNode
{
    void* key;
    void* value;
    struct _bucketNode* next;
} bucketNode;

typedef struct _bucket
{
    bucketNode* head;
    uint64_t hash;
    bool taken;
} bucket;

const float DEFAULT_MAX_LOAD_FACTOR = 0.5f;
struct _hashtable
{
    bucket* buckets;
    size_t capacity;
    size_t entries;
    size_t totalElements;

    hasher h;
    comparator c;

    float maxLoadFactor;
};

// MurmurHash3 http://stackoverflow.com/questions/5085915/what-is-the-best-hash-function-for-uint64-t-keys-ranging-from-0-to-its-max-value
uint64_t hasher_uint64_t(void* ptr)
{
    uint64_t value = *((const uint64_t*) ptr);

    value ^= value >> 33;
    value *= 0xff51afd7ed558ccd;
    value ^= value >> 33;
    value *= 0xc4ceb9fe1a85ec53;
    value ^= value >> 33;

    return value;
}

bool comparator_uint64_t(void* p1, void* p2)
{
    if (!p1 || !p2)
    {
        return false;
    }

    return *((uint64_t*) p1) == *((uint64_t*) p2);
}

uint64_t hasher_ptr(void* ptr)
{
    return hasher_uint64_t((void*) &ptr);
}

bool comparator_ptr(void* p1, void* p2)
{
    return p1 == p2;
}

uint64_t hasher_string(void* ptr)
{
    const char* str = (const char*) ptr;

    uint64_t hash = 5381;
    while (*str)
    {
        hash = ((hash << 5) + hash) * *str;
        str++;
    }

    return hash;
}

bool comparator_string(void* p1, void* p2)
{
    return strcmp((const char*) p1, (const char*) p2) == 0;
}

hashtable* create_hashtable(size_t capacity, hasher h, comparator c)
{
    if (capacity == 0 || !h || !c)
    {
        return NULL;
    }

    hashtable* table = malloc(sizeof(hashtable));
    if (!table)
    {
        return NULL;
    }

    table->buckets = calloc(capacity, sizeof(bucket));
    if (!table->buckets)
    {
        free(table);

        return NULL;
    }

    table->capacity = capacity;
    table->entries = 0;
    table->totalElements = 0;

    table->h = h;
    table->c = c;

    table->maxLoadFactor = DEFAULT_MAX_LOAD_FACTOR;

    return table;
}

bool free_hashtable(hashtable* table)
{
    if (!table)
    {
        return false;
    }

    for (size_t i = 0; i < table->capacity; i++)
    {
        bucketNode* it = table->buckets[i].head;
        bucketNode* next;
        while (it)
        {
            next = it->next;
            free(it);
            it = next;
        }
    }

    free(table);

    return true;
}

bool _getHashIndex_hashtable(hashtable* table, uint64_t hash, size_t* outIndex)
{
    *outIndex = hash % table->capacity;

    size_t step = 1;
    while (table->buckets[*outIndex].hash != hash)
    {
        // If we ever encounter a hole in the table, we know the element has never been inserted
        if (!table->buckets[*outIndex].taken)
        {
            return false;
        }

        // Increase index by step^2
        *outIndex = (*outIndex + step * step) % table->capacity;
        step++;
    }

    return table->buckets[*outIndex].taken;
}

void* get_hashtable(hashtable* table, void* key)
{
    if (!table || !key)
    {
        return NULL;
    }

    size_t index;
    if (!_getHashIndex_hashtable(table, table->h(key), &index))
    {
        return NULL;
    }

    // Find the key that matches the comparator
    for (bucketNode* it = table->buckets[index].head; it; it = it->next)
    {
        if (table->c(key, it->key))
        {
            return it->value;
        }
    }

    return NULL;
}

bool set_hashtable(hashtable* table, void* key, void* value)
{
    if (!table || !key || !value)
    {
        return false;
    }

    size_t index;
    uint64_t hash = table->h(key);

    // Grow the hash table if a new bucket would increase the load factor past the max allowed
    if (!_getHashIndex_hashtable(table, hash, &index))
    {
        // Resize the table if we breach the max load factor
        if ((table->entries + 1) / ((float) table->capacity) > table->maxLoadFactor)
        {
            // Save the old information
            bucket* oldBuckets = table->buckets;
            size_t oldCapacity = table->capacity;

            // Double how many buckets there are
            table->capacity = table->capacity * 2;
            table->buckets = calloc(table->capacity, sizeof(bucket));

            // Did memory allocation fail
            if (!table->buckets)
            {
                table->capacity = oldCapacity;
                table->buckets = oldBuckets;

                return false;
            }

            // Migrate the buckets from the old table to the new
            for (size_t i = 0; i < oldCapacity; i++)
            {
                // If the bucket is empty, don't reinsert it
                if (!oldBuckets[i].head)
                {
                    continue;
                }

                // Get the new index for the old bucket
                _getHashIndex_hashtable(table, oldBuckets[i].hash, &index);

                table->buckets[index].taken = true;
                table->buckets[index].hash = oldBuckets[i].hash;

                // Move the old bucket to its new home
                table->buckets[index].head = oldBuckets[i].head;
            }

            // Since we changed the table, we need to re-get the index to insert
            _getHashIndex_hashtable(table, hash, &index);
        }

        // We have a new bucket, so set its attributes
        table->buckets[index].taken = true;
        table->buckets[index].hash = hash;

        table->entries++;
    }

    // Insert the new data
    bucketNode* toInsert = malloc(sizeof(bucketNode));
    if (!toInsert)
    {
        return false;
    }

    toInsert->key = key;
    toInsert->value = value;
    toInsert->next = table->buckets[index].head;
    table->buckets[index].head = toInsert;

    table->totalElements++;

    return true;
}

void** getAll_hashtable(hashtable* table)
{
    if (!table || table->totalElements == 0)
    {
        return NULL;
    }

    void** allData = malloc(sizeof(void*) * table->totalElements);
    if (!allData)
    {
        return NULL;
    }

    size_t allDataIndex = 0;

    for (size_t i = 0; i < table->capacity; i++)
    {
        for (bucketNode* it = table->buckets[i].head; it; it = it->next)
        {
            allData[allDataIndex++] = it->value;
        }
    }

    return allData;
}

size_t getCount_hashtable(hashtable* table)
{
    return table->totalElements;
}

void* remove_hashtable(hashtable* table, void* key)
{
    if (!table || !key)
    {
        return NULL;
    }

    size_t index;
    if (!_getHashIndex_hashtable(table, table->h(key), &index))
    {
        return NULL;
    }

    bucketNode* it = table->buckets[index].head;
    if (!it)
    {
        return NULL;
    }

    if (table->c(key, it->key))
    {
        table->buckets[index].head = it->next;
        void* result = it->value;
        free(it);
        return result;
    }

    for (bucketNode* prev = it->next; it; it = it->next)
    {
        if (table->c(key, it->key))
        {
            prev = it->next;
            void* result = it->value;
            free(it);
            return result;
        }
    }

    return NULL;
}

void clear_hashtable(hashtable* table)
{
    if (!table)
    {
        return;
    }

    for (size_t i = 0; i < table->capacity; i++)
    {
        bucketNode* it = table->buckets[i].head;
        bucketNode* next;
        while (it)
        {
            next = it->next;
            free(it);
            it = next;
        }

        memset(&table->buckets[i], 0, sizeof(bucket));
    }

    table->entries = 0;
    table->totalElements = 0;
}
