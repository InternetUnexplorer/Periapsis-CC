#ifndef VEC_H
#define VEC_H

#include <stdlib.h>

typedef struct Vec {
    size_t length, capacity;
    void **items;
} Vec;

Vec *vec_new(void);
void vec_free(Vec*);
void vec_clear(Vec*);
size_t vec_len(Vec*);

void *vec_get(Vec*, size_t);
void vec_set(Vec*, size_t, void*);

void vec_insert(Vec*, size_t, void*);
void *vec_remove(Vec*, size_t);

void vec_pushf(Vec*, void*);
void vec_pushb(Vec*, void*);
void *vec_popf(Vec*);
void *vec_popb(Vec*);

#endif
