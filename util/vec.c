#include "vec.h"
#include "alloc.h"

#include <string.h>

static void _resize(Vec *vec, size_t capacity) {
    vec->items = checked_realloc(vec->items, capacity * sizeof(void *));
    vec->capacity = capacity;
}

Vec *vec_new() {
    Vec *vec = MALLOC(Vec);
    vec->length = 0;
    vec->capacity = 0;
    vec->items = NULL;
    return vec;
}

Vec *vec_from(void *item) {
    Vec *vec = vec_new();
    vec_pushf(vec, item);
    return vec;
}

void vec_free(Vec *vec) {
    vec_clear(vec);
    free(vec);
}

void vec_clear(Vec *vec) {
    while (vec->length > 0)
        free(vec->items[--vec->length]);
    _resize(vec, 2);
}

size_t vec_len(Vec *vec) { return vec->length; }

void *vec_get(Vec *vec, size_t index) { return vec->items[index]; }

void vec_set(Vec *vec, size_t index, void *item) { vec->items[index] = item; }

void vec_insert(Vec *vec, size_t index, void *item) {
    if (vec->length == vec->capacity)
        _resize(vec, vec->capacity != 0 ? vec->capacity * 2 : 2);
    memmove(&vec->items[index + 1], &vec->items[index],
            (vec->length++ - index) * sizeof(void *));
    vec->items[index] = item;
}

void *vec_remove(Vec *vec, size_t index) {
    void *item = vec->items[index];
    memmove(&vec->items[index], &vec->items[index + 1],
            (--vec->length - index) * sizeof(void *));
    if (vec->length < vec->capacity / 4)
        _resize(vec, vec->capacity / 2);
    return item;
}

void vec_pushf(Vec *vec, void *item) { vec_insert(vec, 0, item); }

void vec_pushb(Vec *vec, void *item) { vec_insert(vec, vec->length, item); }

void *vec_popf(Vec *vec) {
    return vec->length != 0 ? vec_remove(vec, 0) : NULL;
}

void *vec_popb(Vec *vec) {
    return vec->length != 0 ? vec_remove(vec, vec->length - 1) : NULL;
}
