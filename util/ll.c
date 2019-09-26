#include "ll.h"
#include "alloc.h"

static LLNode *_newnode(void *item) {
    LLNode *node = MALLOC(LLNode);
    node->item = item;
    node->next = NULL;
    return node;
}

static LLNode *_getnode(LL *list, size_t index) {
    LLNode *node = list->head;
    while (index--)
        node = node->next;
    return node;
}

static LLNode *_gettailnode(LL *list) {
    LLNode *node = list->head;
    if (node != NULL)
        while (node->next != NULL)
            node = node->next;
    return node;
}

static void *_unwrap(LLNode *node) {
    void *item = node->item;
    free(node);
    return item;
}

LL *ll_new() {
    LL *list = MALLOC(LL);
    list->head = NULL;
    return list;
}

LL *ll_from(void *item) {
    LL *list = ll_new();
    ll_pushf(list, item);
    return list;
}

void ll_free(LL *list) {
    ll_clear(list);
    free(list);
}

void ll_clear(LL *list) {
    void *item;
    while ((item = ll_popf(list)) != NULL)
        free(item);
}

size_t ll_len(LL *list) {
    size_t len = 0;
    for (LLNode *node = list->head; node != NULL; node = node->next)
        len++;
    return len;
}

void *ll_get(LL *list, size_t index) { return _getnode(list, index)->item; }

void ll_set(LL *list, size_t index, void *item) {
    _getnode(list, index)->item = item;
}

void *ll_head(LL *list) {
    LLNode *head = list->head;
    return head != NULL ? head->item : NULL;
}

void *ll_tail(LL *list) {
    LLNode *tail = _gettailnode(list);
    return tail != NULL ? tail->item : NULL;
}

void ll_insert(LL *list, size_t index, void *item) {
    if (index == 0) {
        ll_pushf(list, item);
    } else {
        LLNode *node = _newnode(item);
        LLNode *prev = _getnode(list, index - 1);
        node->next = prev->next;
        prev->next = node;
    }
}

void *ll_remove(LL *list, size_t index) {
    if (index == 0)
        return ll_popf(list);
    LLNode *prev = _getnode(list, index - 1);
    LLNode *node = prev->next;
    prev->next = node->next;
    return _unwrap(node);
}

void ll_pushf(LL *list, void *item) {
    LLNode *node = _newnode(item);
    node->next = list->head;
    list->head = node;
}

void ll_pushb(LL *list, void *item) {
    LLNode *node = _newnode(item);
    LLNode *tail = _gettailnode(list);
    if (tail == NULL)
        list->head = node;
    else
        tail->next = node;
}

void *ll_popf(LL *list) {
    LLNode *head = list->head;
    if (head == NULL)
        return NULL;
    list->head = head->next;
    return _unwrap(head);
}

void *ll_popb(LL *list) {
    LLNode *prev = list->head;
    if (prev == NULL || prev->next == NULL)
        return ll_popf(list);
    LLNode *tail = prev->next;
    while (tail->next != NULL) {
        prev = tail;
        tail = tail->next;
    }
    prev->next = NULL;
    return _unwrap(tail);
}
