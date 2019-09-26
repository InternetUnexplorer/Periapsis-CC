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
LL *ll_from(void *item);
void ll_free(LL *list);
void ll_clear(LL *list);
size_t ll_len(LL *list);

void *ll_get(LL *list, size_t index);
void ll_set(LL *list, size_t index, void *item);

void *ll_head(LL *list);
void *ll_tail(LL *list);

void ll_insert(LL *list, size_t index, void *item);
void *ll_remove(LL *list, size_t index);

void ll_pushf(LL *list, void *item);
void ll_pushb(LL *list, void *item);
void *ll_popf(LL *list);
void *ll_popb(LL *list);

#endif
