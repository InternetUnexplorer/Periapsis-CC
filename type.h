#ifndef TYPE_H
#define TYPE_H

#include "decl_spec.h"

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

Type type_from_specs(DeclSpecs specs);
void type_fmt(Type type, FILE *stream);

#endif
