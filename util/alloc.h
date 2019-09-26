#ifndef ALLOC_H
#define ALLOC_H

#include <stdlib.h>

#define MALLOC(TYPE) (TYPE *) checked_malloc(sizeof(TYPE))

void *checked_malloc(size_t size);
void *checked_realloc(void *ptr, size_t size);

#endif
