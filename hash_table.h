#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include "unnecessary.h"

// inspired by this write-up
// https://benhoyt.com/writings/hash-table-in-c/
// by
// https://github.com/benhoyt

// FNV-1a hash generator
#define FNV_OFFSET_BASIS 14695981039346656037UL
#define FNV_PRIME 1099511628211UL
// https://en.wikipedia.org/wiki/Fowler-Noll-Vo_hash_function
inline uint64_t UNNECESSARY_FNV1A_HASH(const char *key)
{
	uint64_t hash = FNV_OFFSET_BASIS;
	// iterate each byte in the string
	for (const char *byte = key; *byte; byte++)
	{
		// cast the byte as unsigned 64 bit int
		// and XOR with hash, modifying the lower 8 bits
		hash ^= (uint64_t)*byte;
		// (important because the order of these operations is
		// the difference between FNV-1 and FNV-1a)
		hash *= FNV_PRIME;
		// hash is now the lower 64 bits of the above product
	}

	return hash;
}

typedef struct UNNECESSARY_HASH_BUCKET
{
	const char *key;
	UNNECESSARY_T_LINKED_NODE *value;
} UNNECESSARY_T_HASH_BUCKET;

static UNNECESSARY_T_DYNAMIC_ARRAY *UNNECESSARY_HASH_TABLE_create()
{
	UNNECESSARY_T_DYNAMIC_ARRAY *table = UNNECESSARY_DYNAMIC_ARRAY_create();

	return table;
}

static void UNNECESSARY_HASH_TABLE_free(UNNECESSARY_T_DYNAMIC_ARRAY **table)
{
	for (int i = 0; i < UNNECESSARY_DYNAMIC_ARRAY_capacity(table); i++)
	{
		const UNNECESSARY_T_HASH_BUCKET *bucket = UNNECESSARY_DYNAMIC_ARRAY_get(table, i);

		if (bucket)
			free((void *)bucket->key);
	}

	UNNECESSARY_DYNAMIC_ARRAY_free(table);
	// free(table);
}

static int UNNECESSARY_HASH_TABLE_length(UNNECESSARY_T_DYNAMIC_ARRAY **table)
{
	return UNNECESSARY_DYNAMIC_ARRAY_size(table);
}

static void *UNNECESSARY_HASH_TABLE_get(
	UNNECESSARY_T_DYNAMIC_ARRAY **table,
	const char *key
)
{
	const uint64_t hash = UNNECESSARY_FNV1A_HASH(key);
	const int capacity = UNNECESSARY_DYNAMIC_ARRAY_capacity(table);
	int index = hash & (capacity - 1);

	UNNECESSARY_T_HASH_BUCKET *bucket = UNNECESSARY_DYNAMIC_ARRAY_get(table, index);

	if (!bucket)
		return NULL;

	while (bucket->key != NULL)
	{

		if (0 == strcmp(key, bucket->key))
			return bucket->value;

		if (++index >= capacity)
			index = 0;

		bucket = UNNECESSARY_DYNAMIC_ARRAY_get(table, index);
	}

	return NULL;
}

#endif //HASH_TABLE_H
