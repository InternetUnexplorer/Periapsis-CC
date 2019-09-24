#ifndef AST_H
#define AST_H

typedef enum AstNodeType {
    AST_L_INT,
    AST_IDENT,
    AST_BINOP,
} AstNodeType;

typedef struct AstNode {
    AstNodeType type;
    union {
        struct { int int_val; }; /* AST_L_INT */
        struct { char *ident; }; /* AST_IDENT */
        struct { char op;        /* AST_BINOP */
                 struct AstNode *lhs, *rhs; };
    };
} AstNode;

AstNode *ast_l_int(int int_val);
AstNode *ast_ident(char *ident);
AstNode *ast_binop(char op, AstNode *lhs, AstNode *rhs);

void ast_free(AstNode *node);
void ast_printf(AstNode *node);

#endif
