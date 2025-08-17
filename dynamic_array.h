#ifndef UNNECESSARY_DYNAMIC_ARRAY_H
#define UNNECESSARY_DYNAMIC_ARRAY_H
#include <stdlib.h>

/*
 * heavily inspired by the article at
 * https://eddmann.com/posts/implementing-a-dynamic-vector-array-in-c/
 * by
 * https://github.com/eddmann
 */

extern const int UNNECESSARY_DYNAMIC_ARRAY_DEFAULT_CREATE_CAPACITY = 4;

// todo
// consider renaming this... maybe `dynamic_array`
// it's inevitable that this library will have math and vec2/vec3 structs
// manually changing any field on this struct can produce undefined behavior
// pls don't
struct UNNECESSARY_DYNAMIC_ARRAY
{
    void **things;
    // 64 bit - no reason not to have these as ints
    // if ever a 32 bit version, they could conditionally be shorts
    int size;
    int capacity;
};

inline void UNNECESSARY_DYNAMIC_ARRAY_create(struct UNNECESSARY_DYNAMIC_ARRAY *vector)
{
    vector->size = 0;
    vector->capacity = UNNECESSARY_DYNAMIC_ARRAY_DEFAULT_CREATE_CAPACITY;
    vector->things = malloc(
        sizeof(void *) * vector->capacity
    );
}

inline void UNNECESSARY_DYNAMIC_ARRAY_free(const struct UNNECESSARY_DYNAMIC_ARRAY *vector)
{
    free(vector->things);
}

inline int UNNECESSARY_DYNAMIC_ARRAY_size(const struct UNNECESSARY_DYNAMIC_ARRAY *vector)
{
    return vector->size;
}

inline int UNNECESSARY_DYNAMIC_ARRAY_capacity(const struct UNNECESSARY_DYNAMIC_ARRAY *vector)
{
    return vector->capacity;
}

inline void UNNECESSARY_DYNAMIC_ARRAY_resize(
    struct UNNECESSARY_DYNAMIC_ARRAY *vector,
    const int capacity
)
{
    void **things = realloc(
        vector->things,
        sizeof(void *) * capacity
    );

    if (things)
    {
        vector->things = things;
        vector->capacity = capacity;
    }
}

inline void UNNECESSARY_DYNAMIC_ARRAY_add(
    struct UNNECESSARY_DYNAMIC_ARRAY *vector,
    void *thing
)
{
    if (vector->capacity == vector->size)
        UNNECESSARY_DYNAMIC_ARRAY_resize(vector, vector->capacity * 2);

    vector->things[vector->size++] = thing;
}

inline void UNNECESSARY_DYNAMIC_ARRAY_set(
    const struct UNNECESSARY_DYNAMIC_ARRAY *vector,
    const int index,
    void * thing
)
{
    if (
           index >= 0
        && index < vector->size
    )
        vector->things[index] = thing;
}

inline void *UNNECESSARY_DYNAMIC_ARRAY_get(
    const struct UNNECESSARY_DYNAMIC_ARRAY *vector,
    const int index
)
{
    if (
           index >= 0
        && index < vector->size
    )
        return vector->things[index];

    return NULL;
}

inline void UNNECESSARY_DYNAMIC_ARRAY_delete(
    struct UNNECESSARY_DYNAMIC_ARRAY *vector,
    const int index
)
{
    if (
           index < 0
        || index >= vector->size
    )
        return;

    vector->things[index] = NULL;

    // slide every item right of index to the left
    // NULL the old last item
    for (int i = index; i < vector->size - 1; i++)
    {
        vector->things[i] = vector->things[i + 1];
        vector->things[i + 1] = NULL;
    }

    // if the capacity can be halved and still be twice the size,
    // vector can resize to half capacity
    if (
         --vector->size > 0
        && vector->size == vector->capacity / 4
    )
        UNNECESSARY_DYNAMIC_ARRAY_resize(vector, vector->capacity / 2);
}
#endif //UNNECESSARY_DYNAMIC_ARRAY_H