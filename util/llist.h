#ifndef LL_H
#define LL_H

#include <stdlib.h>

typedef struct LL {
    struct LLNode *head;
} LL;

typedef struct LLNode {
    void *item;
    struct LLNode *next;
} LLNode;

LL *ll_new(void);
void ll_free(LL*);
void ll_clear(LL*);
size_t ll_len(LL*);

void *ll_head(LL*);
void *ll_tail(LL*);

void ll_insert(LL*, size_t, void*);
void *ll_remove(LL*, size_t);

void ll_pushf(LL*, void*);
void ll_pushb(LL*, void*);
void *ll_popf(LL*);
void *ll_popb(LL*);

#endif
