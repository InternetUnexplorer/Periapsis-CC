#include "type.h"

Type type_from_specs(DeclSpecs specs) {
    if (specs & (DECL_SPEC_INT | DECL_SPEC_SIGN)) {
        bool is_signed = !(specs & DECL_SPEC_UNSIGNED);
        return (Type){ .kind = TYPE_INT, ._.is_signed = is_signed };
    } else {
        return (Type){ .kind = TYPE_VOID };
    }
}

void type_fmt(Type type, FILE *stream) {
    switch (type.kind) {
    case TYPE_INT:
        fprintf(stream, type._.is_signed ? "int" : "unsigned int");
        break;
    case TYPE_VOID:
        fprintf(stream, "void");
        break;
    default:
        fprintf(stream, "/* unknown */");
        break;
    }
}
