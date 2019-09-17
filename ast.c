#include "ast.h"

#include <stdlib.h>
#include <stdio.h>

static AstNode *_new(AstNodeType type) {
    AstNode *node = (AstNode*) malloc(sizeof(AstNode));
    if (node != NULL)
        node->type = type;
    return node;
}

AstNode *ast_l_int(int int_val) {
    AstNode *node = _new(AST_L_INT);
    if (node != NULL)
        node->int_val = int_val;
    return node;
}

AstNode *ast_ident(char* ident) {
    AstNode *node = _new(AST_IDENT);
    if (node != NULL)
        node->ident = ident;
    return node;
}

AstNode *ast_binop(char op, AstNode *lhs, AstNode *rhs) {
    AstNode *node = _new(AST_BINOP);
    if (node != NULL) {
        node->op = op;
        node->rhs = rhs;
        node->lhs = lhs;
    }
    return node;
}

void ast_free(AstNode *node) {
    if (node == NULL)
        return;

    switch (node->type) {
        case AST_L_INT:
            break;
        case AST_IDENT:
            free(node->ident);
            break;
        case AST_BINOP:
            ast_free(node->lhs);
            ast_free(node->rhs);
            break;
    }
    free(node);
}

static void _ast_printf(AstNode *node, int indent) {
    printf("AstNode {\n");
    indent += 4;
    switch (node->type) {
        case AST_L_INT:
            printf("%*stype = AST_L_INT\n", indent, "");
            printf("%*sint_val = %d\n", indent, "", node->int_val);
            break;
        case AST_IDENT:
            printf("%*stype = AST_IDENT\n", indent, "");
            printf("%*sident = \"%s\"\n", indent, "", node->ident);
            break;
        case AST_BINOP:
            printf("%*stype = AST_BINOP\n", indent, "");
            printf("%*sop = '%c'\n", indent, "", node->op);
            printf("%*slhs = ", indent, "");
            _ast_printf(node->lhs, indent);
            printf("%*srhs = ", indent, "");
            _ast_printf(node->rhs, indent);
            break;
        default:
            printf("%*stype = <unknown>\n", indent, "");
            break;
    }
    indent -= 4;
    printf("%*s}\n", indent, "");
}

void ast_printf(AstNode *node) {
    _ast_printf(node, 0);
}
