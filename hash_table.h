#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include "unnecessary.h"
/*
 * inspired by this write-up
 * https://benhoyt.com/writings/hash-table-in-c/
 * by
 * https://github.com/benhoyt
 */

// https://en.wikipedia.org/wiki/Fowler-Noll-Vo_hash_function
#define FNV_OFFSET_BASIS 14695981039346656037UL
#define FNV_PRIME 1099511628211UL
// FNV-1a hash generator
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

const int UNNECESSARY_DEFAULT_HASH_TABLE_CAPACITY = 16;

typedef struct UNNECESSARY_HASH_BUCKET
{
	const char *key;
	void *value;
} UNNECESSARY_T_HASH_BUCKET;

typedef struct UNNECESSARY_HASH_TABLE
{
	UNNECESSARY_T_HASH_BUCKET *things;
	int size;
	int capacity;
} UNNECESSARY_T_HASH_TABLE;

static UNNECESSARY_T_HASH_TABLE *UNNECESSARY_HASH_TABLE_create()
{
	UNNECESSARY_T_HASH_TABLE *table = malloc(sizeof(*table));
	table->things = calloc(UNNECESSARY_DEFAULT_HASH_TABLE_CAPACITY, sizeof(*table->things));
	table->size = 0;
	table->capacity = UNNECESSARY_DEFAULT_HASH_TABLE_CAPACITY;
	return table;
}



static int UNNECESSARY_HASH_TABLE_size(UNNECESSARY_T_HASH_TABLE **table)
{
	return (*table)->size;
}

static int UNNECESSARY_HASH_TABLE_capacity(UNNECESSARY_T_HASH_TABLE **table)
{
	return (*table)->capacity;
}

static void UNNECESSARY_HASH_TABLE_free(UNNECESSARY_T_HASH_TABLE **table)
{
	for (int i = 0; i < (*table)->capacity; i++)
	{
		UNNECESSARY_T_HASH_BUCKET *bucket = &(*table)->things[i];
		if (bucket->key)
		{
			bucket->value = NULL;
		}
	}
	free((*table)->things);
}

static int UNNECESSARY_HASH_TABLE_length(UNNECESSARY_T_HASH_TABLE **table)
{
	return UNNECESSARY_HASH_TABLE_size(table);
}

static void *UNNECESSARY_HASH_TABLE_get(
	UNNECESSARY_T_HASH_TABLE **table,
	const char *key
)
{
	short index = UNNECESSARY_FNV1A_HASH(key) & (uint64_t)((*table)->capacity - 1);
	UNNECESSARY_T_HASH_BUCKET *bucket = &(*table)->things[index];;
	if (!bucket)
	{
		return NULL;
	}
	while (bucket->key != NULL)
	{
		if (0 == strcmp(key, bucket->key))
		{
			return bucket->value;
		}
		if (++index >= (*table)->capacity)
		{
			index = 0;
		}
		bucket = &(*table)->things[index];
	}
	return NULL;
}

static const char *UNNECESSARY_HASH_TABLE_index(
	UNNECESSARY_T_HASH_BUCKET *things, const int capacity, const char *key, UNNECESSARY_T_LINKED_NODE *value, int *size
)
{
	short index = UNNECESSARY_FNV1A_HASH(key) & (uint64_t)(capacity - 1);
	while (things[index].key != NULL)
	{
		if (0 == strcmp(key, things[index].key))
		{
			things[index].value = value;
			return things[index].key;
		}
		index++;
		if (index >= capacity)
		{
			index = 0;
		}
	}
	if (size != NULL)
	{
		if (key == NULL)
		{
			return NULL;
		}
		(*size)++;
	}
	things[index].key = key;
	things[index].value = value;
	return key;
}

static int UNNECESSARY_HASH_TABLE_resize(UNNECESSARY_T_HASH_TABLE **table)
{
	const int capacity = (*table)->capacity * 2;
	if (capacity < (*table)->capacity)
	{
		return 1;
	}
	UNNECESSARY_T_HASH_BUCKET *things = calloc(capacity, sizeof(*things));
	if (things == NULL)
	{
		return 2;
	}
	for (int i = 0; i < (*table)->capacity; i++)
	{
		const UNNECESSARY_T_HASH_BUCKET *bucket = &(*table)->things[i];
		if (bucket->key != NULL)
		{
			if (!UNNECESSARY_HASH_TABLE_index(
				things, capacity, bucket->key, bucket->value, NULL
			))
			{
				return 3;
			}
		}
	}
	free((*table)->things);
	(*table)->things = things;
	(*table)->capacity = capacity;
	return 0;
}

static const char *UNNECESSARY_HASH_TABLE_set(
	UNNECESSARY_T_HASH_TABLE **table,
	const char *key,
	void *value
)
{
	if (value == NULL)
	{
		return NULL;
	}
	if ((*table)->size >= (*table)->capacity / 2)
	{
		if (UNNECESSARY_HASH_TABLE_resize(table) > 0)
		{
			return NULL;
		}
	}
	return UNNECESSARY_HASH_TABLE_index(
		(*table)->things, (*table)->capacity, key, value, &(*table)->size
	);
}

#endif //HASH_TABLE_H
