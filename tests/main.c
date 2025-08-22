#include <assert.h>
#include <string.h>

#include "../unnecessary.h"

int main()
{
	UNNECESSARY_T_SERIAL_STRUCTURE *t = UNNECESSARY_SERIAL_STRUCTURE_create();
	UNNECESSARY_SERIAL_FILE_write(t, "game.sav");
	free(t);
	UNNECESSARY_T_HASH_TABLE *table = UNNECESSARY_HASH_TABLE_create();
	UNNECESSARY_HASH_TABLE_length(&table);
	UNNECESSARY_HASH_TABLE_get(&table, "asdf");
	UNNECESSARY_HASH_TABLE_set(&table, "fdas", UNNECESSARY_LINKED_LIST_create());
	UNNECESSARY_T_LINKED_NODE *ll = UNNECESSARY_HASH_TABLE_get(&table, "fdas");
	UNNECESSARY_LINKED_LIST_push(&ll, "asdfasdfasdf");
	UNNECESSARY_HASH_TABLE_set(&table, "fd1as", UNNECESSARY_LINKED_LIST_create());
	UNNECESSARY_HASH_TABLE_set(&table, "fd2as", UNNECESSARY_LINKED_LIST_create());
	UNNECESSARY_HASH_TABLE_set(&table, "fd3as", UNNECESSARY_LINKED_LIST_create());
	UNNECESSARY_HASH_TABLE_set(&table, "fd4as", UNNECESSARY_LINKED_LIST_create());
	UNNECESSARY_HASH_TABLE_set(&table, "fd5as", UNNECESSARY_LINKED_LIST_create());
	UNNECESSARY_HASH_TABLE_set(&table, "fd6as", UNNECESSARY_LINKED_LIST_create());
	UNNECESSARY_HASH_TABLE_set(&table, "fd7as", UNNECESSARY_LINKED_LIST_create());
	UNNECESSARY_HASH_TABLE_set(&table, "fd8as", UNNECESSARY_LINKED_LIST_create());
	UNNECESSARY_HASH_TABLE_set(&table, "fd9as", UNNECESSARY_LINKED_LIST_create());
	UNNECESSARY_HASH_TABLE_set(&table, "fd10as", UNNECESSARY_LINKED_LIST_create());
	UNNECESSARY_HASH_TABLE_set(&table, "fd11as", UNNECESSARY_LINKED_LIST_create());
	UNNECESSARY_HASH_TABLE_set(&table, "fd12as", UNNECESSARY_LINKED_LIST_create());
	UNNECESSARY_HASH_TABLE_set(&table, "fd13as", UNNECESSARY_LINKED_LIST_create());
	UNNECESSARY_HASH_TABLE_set(&table, "fd14as", UNNECESSARY_LINKED_LIST_create());
	UNNECESSARY_HASH_TABLE_set(&table, "fd15as", UNNECESSARY_LINKED_LIST_create());
	UNNECESSARY_HASH_TABLE_set(&table, "fd16as", UNNECESSARY_LINKED_LIST_create());
	UNNECESSARY_HASH_TABLE_set(&table, "fd17as", UNNECESSARY_LINKED_LIST_create());
	UNNECESSARY_HASH_TABLE_free(&table);
	free(table);
	table = NULL;

	UNNECESSARY_T_LINKED_NODE *l = UNNECESSARY_LINKED_LIST_create();
	UNNECESSARY_LINKED_LIST_push(&l, "1");
	UNNECESSARY_LINKED_LIST_push(&l, "2");
	UNNECESSARY_LINKED_LIST_push(&l, "3");
	UNNECESSARY_LINKED_LIST_push(&l, "4");
	printf(UNNECESSARY_LINKED_LIST_pop(&l));
	printf(UNNECESSARY_LINKED_LIST_pop(&l));
	printf(UNNECESSARY_LINKED_LIST_pop(&l));
	printf(UNNECESSARY_LINKED_LIST_pop(&l));
	UNNECESSARY_LINKED_LIST_free(&l);
	l = UNNECESSARY_LINKED_LIST_create();
	// test what happens if we pass a reference to a pointer to NULL
	UNNECESSARY_LINKED_LIST_pop(&l);

	UNNECESSARY_LINKED_LIST_push(&l, "10");
	// UNNECESSARY_LINKED_LIST_push(&l, (void *)1000); // VALGRIND leak 16 bytes
	UNNECESSARY_LINKED_LIST_pop(&l);

	UNNECESSARY_T_DYNAMIC_ARRAY *v = UNNECESSARY_DYNAMIC_ARRAY_create();

	UNNECESSARY_DYNAMIC_ARRAY_add(&v, "a");
	UNNECESSARY_DYNAMIC_ARRAY_add(&v, "ab");
	UNNECESSARY_DYNAMIC_ARRAY_add(&v, "abc");
	UNNECESSARY_DYNAMIC_ARRAY_add(&v, "abcd");

	assert(0 == strcmp("a", UNNECESSARY_DYNAMIC_ARRAY_get(&v, 0)));
	assert(0 == strcmp("ab", UNNECESSARY_DYNAMIC_ARRAY_get(&v, 1)));
	assert(0 == strcmp("abc", UNNECESSARY_DYNAMIC_ARRAY_get(&v, 2)));
	assert(0 == strcmp("abcd", UNNECESSARY_DYNAMIC_ARRAY_get(&v, 3)));

	UNNECESSARY_DYNAMIC_ARRAY_delete(&v, 3);
	UNNECESSARY_DYNAMIC_ARRAY_delete(&v, 2);

	// v->capacity should not have changed yet, so it will still be default capacity
	assert(UNNECESSARY_DEFAULT_DYNAMIC_ARRAY_CAPACITY == UNNECESSARY_DYNAMIC_ARRAY_capacity(&v));

	UNNECESSARY_DYNAMIC_ARRAY_delete(&v, 1);

	// after UNNECESSARY_DYNAMIC_ARRAY_delete here, v->size should be 1
	assert(1 == UNNECESSARY_DYNAMIC_ARRAY_size(&v));

	// now this means that the condition of size == capacity / 4 was true,
	// so capacity should have been internally resized to capacity / 2
	assert(UNNECESSARY_DEFAULT_DYNAMIC_ARRAY_CAPACITY / 2 == UNNECESSARY_DYNAMIC_ARRAY_capacity(&v));

	UNNECESSARY_DYNAMIC_ARRAY_set(&v, 0, "zyx");

	assert(1 == UNNECESSARY_DYNAMIC_ARRAY_size(&v));
	assert(0 == strcmp("zyx", UNNECESSARY_DYNAMIC_ARRAY_get(&v, 0)));

	UNNECESSARY_DYNAMIC_ARRAY_add(&v, "wvu");

	assert(2 == UNNECESSARY_DYNAMIC_ARRAY_size(&v));
	assert(0 == strcmp("wvu", UNNECESSARY_DYNAMIC_ARRAY_get(&v, 1)));

	UNNECESSARY_DYNAMIC_ARRAY_free(&v);

	free(l);
	free(v);

	return 0;
}
