#include "ast.h"
#include "util/alloc.h"

#include <stdio.h>
#include <stdlib.h>

#define _AST_MALLOC(NAME, TYPE)                                                \
    TYPE *node = MALLOC(TYPE);                                                 \
    node->type = NAME;

#define _AST_FREE_NODE(TYPE, FIELD) ast_free((AstNode *) ((TYPE *) node)->FIELD)

#define _AST_FREE_NODE_VEC(TYPE, FIELD)                                        \
    {                                                                          \
        Vec *nodes = ((TYPE *) node)->FIELD;                                   \
        for (size_t i = 0; i < vec_len(nodes); i++)                            \
            ast_free((AstNode *) vec_get(nodes, i));                           \
        vec_free(nodes);                                                       \
    }

#define _AST_PRINTF_TYPE(NAME) printf("%*stype = " #NAME "\n", indent, "")

#define _AST_PRINTF_FIELD(TYPE, FIELD, FORMAT)                                 \
    printf("%*s" #FIELD " = " FORMAT "\n", indent, "", ((TYPE *) node)->FIELD)

#define _AST_PRINTF_NODE(TYPE, FIELD)                                          \
    printf("%*s" #FIELD " = ", indent, "");                                    \
    _printf((AstNode *) ((TYPE *) node)->FIELD, indent);

#define _AST_PRINTF_NODE_VEC(TYPE, FIELD)                                      \
    {                                                                          \
        Vec *nodes = ((TYPE *) node)->FIELD;                                   \
        size_t nodes_len = vec_len(nodes);                                     \
        if (nodes_len == 0) {                                                  \
            printf("%*s" #FIELD " = []\n", indent, "");                        \
        } else {                                                               \
            printf("%*s" #FIELD " = [\n", indent, "");                         \
            indent += INDENT_SPACES;                                           \
            for (size_t i = 0; i < nodes_len; i++) {                           \
                printf("%*s", indent, "");                                     \
                _printf((AstNode *) vec_get(nodes, i), indent);                \
            }                                                                  \
            indent -= INDENT_SPACES;                                           \
            printf("%*s]\n", indent, "");                                      \
        }                                                                      \
    }

AstNode *ast_ident(char *ident) {
    _AST_MALLOC(AST_IDENT, AstIdentNode);
    node->ident = ident;
    return (AstNode *) node;
}

AstNode *ast_i_const(long value) {
    _AST_MALLOC(AST_I_CONST, AstIConstNode);
    node->value = value;
    return (AstNode *) node;
}

AstNode *ast_f_const(double value) {
    _AST_MALLOC(AST_F_CONST, AstFConstNode);
    node->value = value;
    return (AstNode *) node;
}

AstNode *ast_fn_call(AstNode *fn, Vec *args) {
    _AST_MALLOC(AST_FN_CALL, AstFnCallNode);
    node->fn = fn;
    node->args = args;
    return (AstNode *) node;
}

AstNode *ast_unop(Unop op, AstNode *_node) {
    _AST_MALLOC(AST_UNOP, AstUnopNode);
    node->op = op;
    node->node = _node;
    return (AstNode *) node;
}

AstNode *ast_binop(Binop op, AstNode *lhs, AstNode *rhs) {
    _AST_MALLOC(AST_BINOP, AstBinopNode);
    node->op = op;
    node->lhs = lhs;
    node->rhs = rhs;
    return (AstNode *) node;
}

AstNode *ast_decl_list(DeclSpecs specs, Vec *decls) {
    _AST_MALLOC(AST_DECL_LIST, AstDeclListNode);
    node->specs = specs;
    node->decls = decls;
    return (AstNode *) node;
}

AstNode *ast_decl(AstNode *ident, AstNode *value) {
    _AST_MALLOC(AST_DECL, AstDeclNode);
    node->ident = ident;
    node->value = value;
    return (AstNode *) node;
}

AstNode *ast_expr_stmt(AstNode *expr) {
    _AST_MALLOC(AST_EXPR_STMT, AstExprStmtNode);
    node->expr = expr;
    return (AstNode *) node;
}

AstNode *ast_cmpd_stmt(Vec *stmts) {
    _AST_MALLOC(AST_CMPD_STMT, AstCmpdStmtNode);
    node->stmts = stmts;
    return (AstNode *) node;
}

AstNode *ast_if_else_stmt(AstNode *expr, AstNode *if_stmt, AstNode *else_stmt) {
    _AST_MALLOC(AST_IF_ELSE_STMT, AstIfElseStmtNode);
    node->expr = expr;
    node->if_stmt = if_stmt;
    node->else_stmt = else_stmt;
    return (AstNode *) node;
}

AstNode *ast_while_stmt(AstNode *expr, AstNode *stmt) {
    _AST_MALLOC(AST_WHILE_STMT, AstWhileStmtNode);
    node->expr = expr;
    node->stmt = stmt;
    return (AstNode *) node;
}

AstNode *ast_do_while_stmt(AstNode *expr, AstNode *stmt) {
    _AST_MALLOC(AST_DO_WHILE_STMT, AstDoWhileStmtNode);
    node->expr = expr;
    node->stmt = stmt;
    return (AstNode *) node;
}

AstNode *ast_for_stmt(AstNode *init_expr, AstNode *cond_expr,
                      AstNode *incr_expr, AstNode *stmt) {
    _AST_MALLOC(AST_FOR_STMT, AstForStmtNode);
    node->init_expr = init_expr;
    node->cond_expr = cond_expr;
    node->incr_expr = incr_expr;
    node->stmt = stmt;
    return (AstNode *) node;
}

AstNode *ast_trans_unit(Vec *decls) {
    _AST_MALLOC(AST_TRANS_UNIT, AstTransUnitNode);
    node->decls = decls;
    return (AstNode *) node;
}

void ast_free(AstNode *node) {
    if (node == NULL)
        return;
    switch (node->type) {
    case AST_IDENT:
    case AST_I_CONST:
    case AST_F_CONST:
    case AST_STR_LIT:
        break;
    case AST_FN_CALL:
        _AST_FREE_NODE(AstFnCallNode, fn);
        _AST_FREE_NODE_VEC(AstFnCallNode, args);
        break;
    case AST_UNOP:
        _AST_FREE_NODE(AstUnopNode, node);
        break;
    case AST_BINOP:
        _AST_FREE_NODE(AstBinopNode, lhs);
        _AST_FREE_NODE(AstBinopNode, rhs);
        break;
    case AST_DECL_LIST:
        _AST_FREE_NODE_VEC(AstDeclListNode, decls);
        break;
    case AST_DECL:
        _AST_FREE_NODE(AstDeclNode, ident);
        _AST_FREE_NODE(AstDeclNode, value);
        break;
    case AST_EXPR_STMT:
        _AST_FREE_NODE(AstExprStmtNode, expr);
        break;
    case AST_CMPD_STMT:
        _AST_FREE_NODE_VEC(AstCmpdStmtNode, stmts);
        break;
    case AST_IF_ELSE_STMT:
        _AST_FREE_NODE(AstIfElseStmtNode, expr);
        _AST_FREE_NODE(AstIfElseStmtNode, if_stmt);
        _AST_FREE_NODE(AstIfElseStmtNode, else_stmt);
        break;
    case AST_WHILE_STMT:
        _AST_FREE_NODE(AstWhileStmtNode, expr);
        _AST_FREE_NODE(AstWhileStmtNode, stmt);
        break;
    case AST_DO_WHILE_STMT:
        _AST_FREE_NODE(AstDoWhileStmtNode, expr);
        _AST_FREE_NODE(AstDoWhileStmtNode, stmt);
        break;
    case AST_FOR_STMT:
        _AST_FREE_NODE(AstForStmtNode, init_expr);
        _AST_FREE_NODE(AstForStmtNode, cond_expr);
        _AST_FREE_NODE(AstForStmtNode, incr_expr);
        _AST_FREE_NODE(AstForStmtNode, stmt);
        break;
    case AST_TRANS_UNIT:
        _AST_FREE_NODE_VEC(AstTransUnitNode, decls);
        break;
    }
    free(node);
}

static void _printf(AstNode *node, int indent) {
    const int INDENT_SPACES = 4;
    if (node == NULL) {
        printf("NULL\n");
        return;
    }
    printf("AstNode {\n");
    indent += INDENT_SPACES;
    switch (node->type) {
    case AST_IDENT:
        _AST_PRINTF_TYPE(AST_IDENT);
        _AST_PRINTF_FIELD(AstIdentNode, ident, "\"%s\"");
        break;
    case AST_I_CONST:
        _AST_PRINTF_TYPE(AST_I_CONST);
        _AST_PRINTF_FIELD(AstIConstNode, value, "%ld");
        break;
    case AST_F_CONST:
        _AST_PRINTF_TYPE(AST_F_CONST);
        _AST_PRINTF_FIELD(AstFConstNode, value, "%lf");
        break;
    case AST_FN_CALL:
        _AST_PRINTF_TYPE(AST_FN_CALL);
        _AST_PRINTF_NODE(AstFnCallNode, fn);
        _AST_PRINTF_NODE_VEC(AstFnCallNode, args);
        break;
    case AST_UNOP:
        _AST_PRINTF_TYPE(AST_UNOP);
        _AST_PRINTF_NODE(AstUnopNode, node);
        break;
    case AST_BINOP:
        _AST_PRINTF_TYPE(AST_BINOP);
        _AST_PRINTF_NODE(AstBinopNode, lhs);
        _AST_PRINTF_NODE(AstBinopNode, rhs);
        break;
    case AST_DECL_LIST:
        _AST_PRINTF_TYPE(AST_DECL_LIST);
        _AST_PRINTF_FIELD(AstDeclListNode, specs, "0x%02X");
        _AST_PRINTF_NODE_VEC(AstDeclListNode, decls);
        break;
    case AST_DECL:
        _AST_PRINTF_TYPE(AST_DECL);
        _AST_PRINTF_NODE(AstDeclNode, ident);
        _AST_PRINTF_NODE(AstDeclNode, value);
        break;
    case AST_EXPR_STMT:
        _AST_PRINTF_TYPE(AST_EXPR_STMT);
        _AST_PRINTF_NODE(AstExprStmtNode, expr);
        break;
    case AST_CMPD_STMT:
        _AST_PRINTF_TYPE(AST_CMPD_STMT);
        _AST_PRINTF_NODE_VEC(AstCmpdStmtNode, stmts);
        break;
    case AST_IF_ELSE_STMT:
        _AST_PRINTF_TYPE(AST_IF_ELSE_STMT);
        _AST_PRINTF_NODE(AstIfElseStmtNode, expr);
        _AST_PRINTF_NODE(AstIfElseStmtNode, if_stmt);
        _AST_PRINTF_NODE(AstIfElseStmtNode, else_stmt);
        break;
    case AST_WHILE_STMT:
        _AST_PRINTF_TYPE(AST_WHILE_STMT);
        _AST_PRINTF_NODE(AstWhileStmtNode, expr);
        _AST_PRINTF_NODE(AstWhileStmtNode, stmt);
        break;
    case AST_DO_WHILE_STMT:
        _AST_PRINTF_TYPE(AST_DO_WHILE_STMT);
        _AST_PRINTF_NODE(AstDoWhileStmtNode, expr);
        _AST_PRINTF_NODE(AstDoWhileStmtNode, stmt);
        break;
    case AST_FOR_STMT:
        _AST_PRINTF_TYPE(AST_FOR_STMT);
        _AST_PRINTF_NODE(AstForStmtNode, init_expr);
        _AST_PRINTF_NODE(AstForStmtNode, cond_expr);
        _AST_PRINTF_NODE(AstForStmtNode, incr_expr);
        _AST_PRINTF_NODE(AstForStmtNode, stmt);
        break;
    case AST_TRANS_UNIT:
        _AST_PRINTF_TYPE(AST_TRANS_UNIT);
        _AST_PRINTF_NODE_VEC(AstTransUnitNode, decls);
        break;
    default:
        _AST_PRINTF_TYPE("/* unknown */");
        break;
    }
    indent -= INDENT_SPACES;
    printf("%*s}\n", indent, "");
}

void ast_printf(AstNode *node) { _printf(node, 0); }
