#include "type.h"

void type_fmt(Type type, FILE *stream) {
    switch (type.kind) {
    case TYPE_INT:
        if (!type._.is_signed)
            fprintf(stream, "unsigned ");
        fprintf(stream, "int");
        break;
    case TYPE_FLOAT:
        fprintf(stream, "float");
        break;
    case TYPE_VOID:
        fprintf(stream, "void");
        break;
    default:
        fprintf(stream, "/* unknown */");
        break;
    }
}
