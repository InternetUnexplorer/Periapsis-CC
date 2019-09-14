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

AstNode *ast_l_int(int);
AstNode *ast_ident(char*);
AstNode *ast_binop(char, AstNode*, AstNode*);

void ast_free(AstNode*);
void ast_printf(AstNode*);

#endif
