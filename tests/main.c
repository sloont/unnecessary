#include <assert.h>
#include <string.h>

#include "../library.h"
int main()
{
	struct UNNECESSARY_VECTOR v;

	UNNECESSARY_VECTOR_create(&v);

	UNNECESSARY_VECTOR_add(&v, "a");
	UNNECESSARY_VECTOR_add(&v, "ab");
	UNNECESSARY_VECTOR_add(&v, "abc");
	UNNECESSARY_VECTOR_add(&v, "abcd");

	assert(0 == strcmp("a", UNNECESSARY_VECTOR_get(&v, 0)));
	assert(0 == strcmp("ab", UNNECESSARY_VECTOR_get(&v, 1)));
	assert(0 == strcmp("abc", UNNECESSARY_VECTOR_get(&v, 2)));
	assert(0 == strcmp("abcd", UNNECESSARY_VECTOR_get(&v, 3)));

	UNNECESSARY_VECTOR_delete(&v, 3);
	UNNECESSARY_VECTOR_delete(&v, 2);

	// v->capacity should not have changed yet, so it will still be default capacity
	assert(UNNECESSARY_VECTOR_DEFAULT_CREATE_CAPACITY == UNNECESSARY_VECTOR_capacity(&v));

	UNNECESSARY_VECTOR_delete(&v, 1);

	// after UNNECESSARY_VECTOR_delete here, v->size should be 1
	assert(1 == UNNECESSARY_VECTOR_size(&v));

	// now this means that the condition of size == capacity / 4 was true,
	// so capacity should have been internally resized to capacity / 2
	assert(UNNECESSARY_VECTOR_DEFAULT_CREATE_CAPACITY / 2 == UNNECESSARY_VECTOR_capacity(&v));

	UNNECESSARY_VECTOR_set(&v, 0, "zyx");

	assert(1 == UNNECESSARY_VECTOR_size(&v));
	assert(0 == strcmp("zyx", UNNECESSARY_VECTOR_get(&v, 0)));

	UNNECESSARY_VECTOR_add(&v, "wvu");

	assert(2 == UNNECESSARY_VECTOR_size(&v));
	assert(0 == strcmp("wvu", UNNECESSARY_VECTOR_get(&v, 1)));

	UNNECESSARY_VECTOR_free(&v);

	return 0;
}
