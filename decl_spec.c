#include "decl_spec.h"

const char *decl_spec_str(DeclSpec spec) {
    switch (spec) {
    case DECL_SPEC_FLOAT:
        return "float";
    case DECL_SPEC_INT:
        return "int";
    case DECL_SPEC_SIGNED:
        return "signed";
    case DECL_SPEC_UNSIGNED:
        return "unsigned";
    case DECL_SPEC_VOID:
        return "void";
    default:
        return "/* unknown */";
    }
}
