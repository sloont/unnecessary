#ifndef UNNECESSARY_DYNAMIC_ARRAY_H
#define UNNECESSARY_DYNAMIC_ARRAY_H
#include <stdlib.h>

/*
 * heavily inspired by the article at
 * https://eddmann.com/posts/implementing-a-dynamic-vector-array-in-c/
 * by
 * https://github.com/eddmann
 */

const int UNNECESSARY_DYNAMIC_ARRAY_DEFAULT_CREATE_CAPACITY = 4;

// manually changing any field on this struct can produce undefined behavior
// pls don't
typedef struct UNNECESSARY_DYNAMIC_ARRAY
{
    void **things;
    // 64 bit - no reason not to have these as ints
    // if ever a 32 bit version, they could conditionally be shorts
    int size;
    int capacity;
} UNNECESSARY_T_DYNAMIC_ARRAY;


static UNNECESSARY_T_DYNAMIC_ARRAY *UNNECESSARY_DYNAMIC_ARRAY_create()
{
    UNNECESSARY_T_DYNAMIC_ARRAY *dynamic_array = malloc(sizeof(*dynamic_array));
    dynamic_array->size = 0;
    dynamic_array->capacity = UNNECESSARY_DYNAMIC_ARRAY_DEFAULT_CREATE_CAPACITY;
    // todo
    // zeroed memory is nice but is it really necessary?
    dynamic_array->things = calloc(dynamic_array->capacity, sizeof(void *));
    return dynamic_array;
}

static void UNNECESSARY_DYNAMIC_ARRAY_free(UNNECESSARY_T_DYNAMIC_ARRAY **dynamic_array)
{
    free((*dynamic_array)->things);
}

static int UNNECESSARY_DYNAMIC_ARRAY_size(UNNECESSARY_T_DYNAMIC_ARRAY **dynamic_array)
{
    return (*dynamic_array)->size;
}

static int UNNECESSARY_DYNAMIC_ARRAY_capacity(UNNECESSARY_T_DYNAMIC_ARRAY **dynamic_array)
{
    return (*dynamic_array)->capacity;
}

static void UNNECESSARY_DYNAMIC_ARRAY_resize(
    UNNECESSARY_T_DYNAMIC_ARRAY **dynamic_array,
    const int capacity
)
{
    void **things = realloc(
        (*dynamic_array)->things,
        sizeof(void *) * capacity
    );

    if (things)
    {
        (*dynamic_array)->things = things;
        (*dynamic_array)->capacity = capacity;
    }
}

static void UNNECESSARY_DYNAMIC_ARRAY_add(
    UNNECESSARY_T_DYNAMIC_ARRAY **dynamic_array,
    void *thing
)
{
    if ((*dynamic_array)->capacity == (*dynamic_array)->size)
        UNNECESSARY_DYNAMIC_ARRAY_resize(dynamic_array, (*dynamic_array)->capacity * 2);

    (*dynamic_array)->things[(*dynamic_array)->size++] = thing;
}

static void UNNECESSARY_DYNAMIC_ARRAY_set(
    UNNECESSARY_T_DYNAMIC_ARRAY **dynamic_array,
    const int index,
    void * thing
)
{
    if (
           index >= 0
        && index < (*dynamic_array)->size
    )
        (*dynamic_array)->things[index] = thing;
}

static void *UNNECESSARY_DYNAMIC_ARRAY_get(
    UNNECESSARY_T_DYNAMIC_ARRAY **dynamic_array,
    const int index
)
{
    if (
           index >= 0
        && index < (*dynamic_array)->size
    )
        return (*dynamic_array)->things[index];

    return NULL;
}

static void UNNECESSARY_DYNAMIC_ARRAY_delete(
    UNNECESSARY_T_DYNAMIC_ARRAY **dynamic_array,
    const int index
)
{
    if (
           index < 0
        || index >= (*dynamic_array)->size
    )
        return;

    (*dynamic_array)->things[index] = NULL;

    // slide every item right of index to the left
    // NULL the old last item
    for (int i = index; i < (*dynamic_array)->size - 1; i++)
    {
        (*dynamic_array)->things[i] = (*dynamic_array)->things[i + 1];
        (*dynamic_array)->things[i + 1] = NULL;
    }

    // if the capacity can be halved and still be twice the size,
    // dynamic_array can resize to half capacity
    if (
         --(*dynamic_array)->size > 0
        && (*dynamic_array)->size == (*dynamic_array)->capacity / 4
    )
        UNNECESSARY_DYNAMIC_ARRAY_resize(dynamic_array, (*dynamic_array)->capacity / 2);
}
#endif //UNNECESSARY_DYNAMIC_ARRAY_H