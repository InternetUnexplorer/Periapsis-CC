#ifndef DECL_H
#define DECL_H

#include <stdint.h>

#define DECL_SPEC_VOID 0x1
#define DECL_SPEC_INT 0x2
#define DECL_SPEC_FLOAT 0x4

#define DECL_SPEC_SIGNED 0x8
#define DECL_SPEC_UNSIGNED 0x10

#define DECL_SPEC_TYPE (DECL_SPEC_VOID | DECL_SPEC_INT | DECL_SPEC_FLOAT)
#define DECL_SPEC_SIGN (DECL_SPEC_SIGNED | DECL_SPEC_UNSIGNED)

typedef uint16_t DeclSpecs;

const char *decl_spec_str(DeclSpecs spec);

#endif
