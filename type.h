#ifndef TYPE_H
#define TYPE_H

#include <stdbool.h>
#include <stdio.h>

typedef enum TypeKind {
    TYPE_INT,
    TYPE_VOID,
} TypeKind;

typedef struct Type {
    TypeKind kind;
    union {
        // TYPE_INT
        bool is_signed;
    } _;
} Type;

void type_fmt(Type type, FILE *stream);

#endif
