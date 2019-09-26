#ifndef VEC_H
#define VEC_H

#include <stdlib.h>

typedef struct Vec {
    size_t length, capacity;
    void **items;
} Vec;

Vec *vec_new(void);
Vec *vec_from(void *item);
void vec_free(Vec *vec);
void vec_clear(Vec *vec);
size_t vec_len(Vec *vec);

void *vec_get(Vec *vec, size_t index);
void vec_set(Vec *vec, size_t index, void *item);

void vec_insert(Vec *vec, size_t index, void *item);
void *vec_remove(Vec *vec, size_t index);

void vec_pushf(Vec *vec, void *item);
void vec_pushb(Vec *vec, void *item);
void *vec_popf(Vec *vec);
void *vec_popb(Vec *vec);

#endif
