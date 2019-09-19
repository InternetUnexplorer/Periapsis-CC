#include "vec.h"

#include <stdbool.h>

static bool _resize(Vec *vec, size_t capacity) {
    if ((vec->items = realloc(vec->items, capacity * sizeof(void*))) != NULL)
        vec->capacity = capacity;
    return vec->items != NULL;
}

Vec *vec_new(void) {
    Vec *vec = (Vec*) malloc(sizeof(Vec));
    if (vec != NULL) {
        vec->length = 0;
        vec->capacity = 0;
        vec->items = NULL;
    }
    return vec;
}

void vec_free(Vec* vec) {
    vec_clear(vec);
    free(vec);
}

void vec_clear(Vec* vec) {
    _resize(vec, 0);
}

size_t vec_len(Vec* vec) {
    return vec->length;
}

void *vec_get(Vec *vec, size_t index) {
    return vec->items[index];
}

void vec_set(Vec *vec, size_t index, void *item) {
    vec->items[index] = item;
}

void vec_insert(Vec* vec, size_t index, void* item) {
    if (vec->length == vec->capacity)
        if (!_resize(vec, vec->capacity != 0 ? vec->capacity * 2 : 2))
            return;
    for (size_t i = vec->length; i > index; i--)
        vec->items[i] = vec->items[i - 1];
    vec->items[index] = item;
    vec->length++;
}

void *vec_remove(Vec *vec, size_t index) {
    void *item = vec->items[index];
    vec->length--;
    for (size_t i = index; i < vec->length; i++)
        vec->items[i] = vec->items[i + 1];
    if (vec->length < vec->capacity / 4)
        _resize(vec, vec->capacity / 2);
    return item;
}

void vec_pushf(Vec* vec, void *item) {
    vec_insert(vec, 0, item);
}

void vec_pushb(Vec *vec, void *item) {
    vec_insert(vec, vec->length, item);
}

void *vec_popf(Vec *vec) {
    return vec->length != 0 ? vec_remove(vec, 0) : NULL;
}

void *vec_popb(Vec* vec) {
    return vec->length != 0 ? vec_remove(vec, vec->length - 1) : NULL;
}
