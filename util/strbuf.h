#ifndef STRBUF_H
#define STRBUF_H

#include <stdlib.h>

typedef struct StrBuf {
    size_t length;
    char *string;
} StrBuf;

// Create a new StrBuf
StrBuf *strbuf_new(void);
// Free a StrBuf, returning the contained string
char *strbuf_unwrap(StrBuf *strbuf);
// Free a StrBuf and the contained string
void strbuf_free(StrBuf *strbuf);
// Grow the buffer and concatenate a string to the end
void strbuf_push(StrBuf *strbuf, char *string);

#endif
