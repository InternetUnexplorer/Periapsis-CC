#ifndef OPS_H
#define OPS_H

typedef enum Unop {
    UNOP_NEG,      // Negation
    UNOP_NOT,      // Bitwise NOT
    UNOP_LNOT,     // Logical NOT
    UNOP_PREINCR,  // Pre-increment
    UNOP_PREDECR,  // Pre-decrement
    UNOP_POSTINCR, // Post-increment
    UNOP_POSTDECR, // Post-decrement
} Unop;

typedef enum Binop {
    BINOP_MUL, // Multiplication
    BINOP_DIV, // Division
    BINOP_ADD, // Addition
    BINOP_SUB, // Subtraction
    BINOP_LT,  // Less than
    BINOP_GT,  // Greater than
    BINOP_LE,  // Less than or equal
    BINOP_GE,  // Greater than or equal
    BINOP_EQ,  // Equal
    BINOP_NE,  // Not equal
} Binop;

#endif
