#ifndef VEC_H
#define VEC_H

#include <stdlib.h>

// Generate declarations for a vector containing items of type TYPE, where NAME
// is the name of the vector type and PREFIX is the prefix of its methods
#define VEC_DECL(NAME, PREFIX, TYPE)                                           \
    typedef struct NAME {                                                      \
        size_t length, capacity;                                               \
        TYPE *items;                                                           \
    } NAME;                                                                    \
                                                                               \
    NAME *PREFIX##_new(void);                                                  \
    NAME *PREFIX##_from(TYPE item);                                            \
    void PREFIX##_free(NAME *vec);                                             \
    void PREFIX##_clear(NAME *vec);                                            \
    size_t PREFIX##_len(NAME *vec);                                            \
    TYPE PREFIX##_get(NAME *vec, size_t index);                                \
    void PREFIX##_set(NAME *vec, size_t index, TYPE item);                     \
    void PREFIX##_insert(NAME *vec, size_t index, TYPE item);                  \
    TYPE PREFIX##_remove(NAME *vec, size_t index);                             \
    void PREFIX##_pushf(NAME *vec, TYPE item);                                 \
    void PREFIX##_pushb(NAME *vec, TYPE item);                                 \
    TYPE PREFIX##_popf(NAME *vec);                                             \
    TYPE PREFIX##_popb(NAME *vec);

// Generate definitions for a vector containing items of type TYPE, where NAME
// is the name of the vector type and PREFIX is the prefix of its methods
// Note that this implementation requires string.h
#define VEC_DEFN(NAME, PREFIX, TYPE)                                           \
    static void _##PREFIX##_resize(NAME *vec, size_t capacity) {               \
        vec->items = checked_realloc(vec->items, capacity * sizeof(TYPE));     \
        vec->capacity = capacity;                                              \
    }                                                                          \
                                                                               \
    NAME *PREFIX##_new() {                                                     \
        NAME *vec = MALLOC(NAME);                                              \
        vec->length = 0;                                                       \
        vec->capacity = 0;                                                     \
        vec->items = NULL;                                                     \
        return vec;                                                            \
    }                                                                          \
                                                                               \
    NAME *PREFIX##_from(TYPE item) {                                           \
        NAME *vec = PREFIX##_new();                                            \
        PREFIX##_pushf(vec, item);                                             \
        return vec;                                                            \
    }                                                                          \
                                                                               \
    void PREFIX##_free(NAME *vec) {                                            \
        PREFIX##_clear(vec);                                                   \
        free(vec);                                                             \
    }                                                                          \
                                                                               \
    void PREFIX##_clear(NAME *vec) {                                           \
        while (vec->length > 0)                                                \
            free(vec->items[--vec->length]);                                   \
        _##PREFIX##_resize(vec, 2);                                            \
    }                                                                          \
                                                                               \
    size_t PREFIX##_len(NAME *vec) {                                           \
        return vec->length;                                                    \
    }                                                                          \
                                                                               \
    TYPE PREFIX##_get(NAME *vec, size_t index) {                               \
        return vec->items[index];                                              \
    }                                                                          \
                                                                               \
    void PREFIX##_set(NAME *vec, size_t index, TYPE item) {                    \
        vec->items[index] = item;                                              \
    }                                                                          \
                                                                               \
    void PREFIX##_insert(NAME *vec, size_t index, TYPE item) {                 \
        if (vec->length == vec->capacity)                                      \
            _##PREFIX##_resize(vec, vec->capacity ? vec->capacity * 2 : 2);    \
        memmove(&vec->items[index + 1], &vec->items[index],                    \
                (vec->length++ - index) * sizeof(TYPE));                       \
        vec->items[index] = item;                                              \
    }                                                                          \
                                                                               \
    TYPE PREFIX##_remove(NAME *vec, size_t index) {                            \
        TYPE item = vec->items[index];                                         \
        memmove(&vec->items[index], &vec->items[index + 1],                    \
                (--vec->length - index) * sizeof(TYPE));                       \
        if (vec->length < vec->capacity / 4)                                   \
            _##PREFIX##_resize(vec, vec->capacity / 2);                        \
        return item;                                                           \
    }                                                                          \
                                                                               \
    void PREFIX##_pushf(NAME *vec, TYPE item) {                                \
        PREFIX##_insert(vec, 0, item);                                         \
    }                                                                          \
                                                                               \
    void PREFIX##_pushb(NAME *vec, TYPE item) {                                \
        PREFIX##_insert(vec, vec->length, item);                               \
    }                                                                          \
                                                                               \
    TYPE PREFIX##_popf(NAME *vec) {                                            \
        return vec->length != 0 ? PREFIX##_remove(vec, 0) : NULL;              \
    }                                                                          \
                                                                               \
    TYPE PREFIX##_popb(NAME *vec) {                                            \
        return vec->length != 0 ? PREFIX##_remove(vec, vec->length - 1)        \
                                : NULL;                                        \
    }

// Generate declarations for the vector type `Vec` with items of type `void *`
VEC_DECL(Vec, vec, void *);

#endif
