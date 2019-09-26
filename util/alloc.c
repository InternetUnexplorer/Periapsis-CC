#include "alloc.h"

#include <stdio.h>

static void _alloc_failed() {
    perror("allocation failed");
    abort();
}

void *checked_malloc(size_t size) {
    void *mem = malloc(size);
    if (mem == NULL && size > 0)
        _alloc_failed();
    return mem;
}

void *checked_realloc(void *ptr, size_t size) {
    void *mem = realloc(ptr, size);
    if (mem == NULL && size > 0)
        _alloc_failed();
    return mem;
}
