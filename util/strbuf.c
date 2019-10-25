#include "strbuf.h"
#include "alloc.h"

#include <string.h>

// Create a new StrBuf
StrBuf *strbuf_new(void) {
    StrBuf *strbuf = MALLOC(StrBuf);
    strbuf->length = 0;
    strbuf->string = NULL;
    return strbuf;
}

// Free a StrBuf, returning the contained string
char *strbuf_unwrap(StrBuf *strbuf) {
    char *string = strbuf->string;
    free(strbuf);
    return string;
}

// Free a StrBuf and the contained string
void strbuf_free(StrBuf *strbuf) {
    free(strbuf_unwrap(strbuf));
}

// Grow the buffer and concatenate a string to the end
void strbuf_push(StrBuf *strbuf, char *string) {
    // Do nothing if string is empty
    if (!string)
        return;
    // Grow buffer (+1 byte for NUL)
    size_t new_length = strbuf->length + strlen(string);
    strbuf->string = checked_realloc(strbuf->string, new_length + 1);
    // TODO: Use strcpy() to avoid unnecessary strlen()
    strcat(strbuf->string, string);
    strbuf->length = new_length;
}
