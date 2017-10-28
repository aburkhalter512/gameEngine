#pragma once

#include "util/unit.h"

typedef struct _hashtable hashtable;

// Don't access these functions directly, as they often have unwritten preconditions
bool _getHashIndex_hashtable(hashtable* table, uint64_t hash, size_t* outIndex);

PROTOTYPE_TEST(create_hashtable);
PROTOTYPE_TEST(setGet_hashtable);
