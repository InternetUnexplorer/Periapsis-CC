#ifndef DECL_H
#define DECL_H

#include <stdint.h>

typedef enum DeclSpec {
    DECL_SPEC_NONE = 0x0,
    DECL_SPEC_VOID = 0x1,
    DECL_SPEC_INT = 0x2,
    DECL_SPEC_FLOAT = 0x4,
    DECL_SPEC_SIGNED = 0x8,
    DECL_SPEC_UNSIGNED = 0xF,
} DeclSpec;

typedef uint8_t DeclSpecs;

#endif
