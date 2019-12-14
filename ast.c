#include "ast.h"
#include "util/alloc.h"

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

#define _AST_FMT_TYPE(NAME) fprintf(stream, "%*stype = " #NAME "\n", indent, "")

#define _AST_FMT_FIELD(TYPE, FIELD, FORMAT)                                    \
    fprintf(stream, "%*s" #FIELD " = " FORMAT "\n", indent, "",                \
            ((TYPE *) node)->FIELD)

#define _AST_FMT_NODE(TYPE, FIELD)                                             \
    fprintf(stream, "%*s" #FIELD " = ", indent, "");                           \
    _fmt((AstNode *) ((TYPE *) node)->FIELD, stream, indent);

#define _AST_FMT_NODE_VEC(TYPE, FIELD)                                         \
    do {                                                                       \
        Vec *nodes = ((TYPE *) node)->FIELD;                                   \
        size_t nodes_len = vec_len(nodes);                                     \
        if (nodes_len == 0) {                                                  \
            fprintf(stream, "%*s" #FIELD " = []\n", indent, "");               \
        } else {                                                               \
            fprintf(stream, "%*s" #FIELD " = [\n", indent, "");                \
            indent += INDENT_SPACES;                                           \
            for (size_t i = 0; i < nodes_len; i++) {                           \
                fprintf(stream, "%*s", indent, "");                            \
                _fmt((AstNode *) vec_get(nodes, i), stream, indent);           \
            }                                                                  \
            indent -= INDENT_SPACES;                                           \
            fprintf(stream, "%*s]\n", indent, "");                             \
        }                                                                      \
    } while (0)

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

AstNode *ast_decl_stmt(DeclSpecs specs, Vec *decls) {
    _AST_MALLOC(AST_DECL_STMT, AstDeclStmtNode);
    node->specs = specs;
    node->decls = decls;
    return (AstNode *) node;
}

AstNode *ast_var_decl(AstNode *ident, AstNode *value) {
    _AST_MALLOC(AST_VAR_DECL, AstVarDeclNode);
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

AstNode *ast_return_stmt(AstNode *expr) {
    _AST_MALLOC(AST_RETURN_STMT, AstReturnStmtNode);
    node->expr = expr;
    return (AstNode *) node;
}

AstNode *ast_fn_decl(AstNode *ident, Vec *params, AstNode *body) {
    _AST_MALLOC(AST_FN_DECL, AstFnDeclNode);
    node->ident = ident;
    node->params = params;
    node->body = body;
    return (AstNode *) node;
}

AstNode *ast_param_decl(DeclSpecs specs, AstNode *ident) {
    _AST_MALLOC(AST_PARAM_DECL, AstParamDeclNode);
    node->specs = specs;
    node->ident = ident;
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
        break;
    case AST_I_CONST:
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
    case AST_DECL_STMT:
        _AST_FREE_NODE_VEC(AstDeclStmtNode, decls);
        break;
    case AST_VAR_DECL:
        _AST_FREE_NODE(AstVarDeclNode, ident);
        _AST_FREE_NODE(AstVarDeclNode, value);
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
    case AST_RETURN_STMT:
        _AST_FREE_NODE(AstReturnStmtNode, expr);
        break;
    case AST_FN_DECL:
        _AST_FREE_NODE(AstFnDeclNode, ident);
        _AST_FREE_NODE_VEC(AstFnDeclNode, params);
        _AST_FREE_NODE(AstFnDeclNode, body);
        break;
    case AST_PARAM_DECL:
        _AST_FREE_NODE(AstParamDeclNode, ident);
        break;
    case AST_TRANS_UNIT:
        _AST_FREE_NODE_VEC(AstTransUnitNode, decls);
        break;
    }
    free(node);
}

static void _fmt(AstNode *node, FILE *stream, int indent) {
    const int INDENT_SPACES = 4;
    if (node == NULL) {
        fprintf(stream, "NULL\n");
        return;
    }
    fprintf(stream, "AstNode {\n");
    indent += INDENT_SPACES;
    switch (node->type) {
    case AST_IDENT:
        _AST_FMT_TYPE(AST_IDENT);
        _AST_FMT_FIELD(AstIdentNode, ident, "\"%s\"");
        break;
    case AST_I_CONST:
        _AST_FMT_TYPE(AST_I_CONST);
        _AST_FMT_FIELD(AstIConstNode, value, "%ld");
        break;
    case AST_FN_CALL:
        _AST_FMT_TYPE(AST_FN_CALL);
        _AST_FMT_NODE(AstFnCallNode, fn);
        _AST_FMT_NODE_VEC(AstFnCallNode, args);
        break;
    case AST_UNOP:
        _AST_FMT_TYPE(AST_UNOP);
        _AST_FMT_FIELD(AstUnopNode, op, "0x%02X");
        _AST_FMT_NODE(AstUnopNode, node);
        break;
    case AST_BINOP:
        _AST_FMT_TYPE(AST_BINOP);
        _AST_FMT_FIELD(AstBinopNode, op, "0x%02X");
        _AST_FMT_NODE(AstBinopNode, lhs);
        _AST_FMT_NODE(AstBinopNode, rhs);
        break;
    case AST_DECL_STMT:
        _AST_FMT_TYPE(AST_DECL_STMT);
        _AST_FMT_FIELD(AstDeclStmtNode, specs, "0x%02X");
        _AST_FMT_NODE_VEC(AstDeclStmtNode, decls);
        break;
    case AST_VAR_DECL:
        _AST_FMT_TYPE(AST_VAR_DECL);
        _AST_FMT_NODE(AstVarDeclNode, ident);
        _AST_FMT_NODE(AstVarDeclNode, value);
        break;
    case AST_EXPR_STMT:
        _AST_FMT_TYPE(AST_EXPR_STMT);
        _AST_FMT_NODE(AstExprStmtNode, expr);
        break;
    case AST_CMPD_STMT:
        _AST_FMT_TYPE(AST_CMPD_STMT);
        _AST_FMT_NODE_VEC(AstCmpdStmtNode, stmts);
        break;
    case AST_IF_ELSE_STMT:
        _AST_FMT_TYPE(AST_IF_ELSE_STMT);
        _AST_FMT_NODE(AstIfElseStmtNode, expr);
        _AST_FMT_NODE(AstIfElseStmtNode, if_stmt);
        _AST_FMT_NODE(AstIfElseStmtNode, else_stmt);
        break;
    case AST_WHILE_STMT:
        _AST_FMT_TYPE(AST_WHILE_STMT);
        _AST_FMT_NODE(AstWhileStmtNode, expr);
        _AST_FMT_NODE(AstWhileStmtNode, stmt);
        break;
    case AST_DO_WHILE_STMT:
        _AST_FMT_TYPE(AST_DO_WHILE_STMT);
        _AST_FMT_NODE(AstDoWhileStmtNode, expr);
        _AST_FMT_NODE(AstDoWhileStmtNode, stmt);
        break;
    case AST_FOR_STMT:
        _AST_FMT_TYPE(AST_FOR_STMT);
        _AST_FMT_NODE(AstForStmtNode, init_expr);
        _AST_FMT_NODE(AstForStmtNode, cond_expr);
        _AST_FMT_NODE(AstForStmtNode, incr_expr);
        _AST_FMT_NODE(AstForStmtNode, stmt);
        break;
    case AST_RETURN_STMT:
        _AST_FMT_TYPE(AST_RETURN_STMT);
        _AST_FMT_NODE(AstReturnStmtNode, expr);
        break;
    case AST_FN_DECL:
        _AST_FMT_TYPE(AST_FN_DECL);
        _AST_FMT_NODE(AstFnDeclNode, ident);
        _AST_FMT_NODE_VEC(AstFnDeclNode, params);
        _AST_FMT_NODE(AstFnDeclNode, body);
        break;
    case AST_PARAM_DECL:
        _AST_FMT_TYPE(AST_PARAM_DECL);
        _AST_FMT_FIELD(AstParamDeclNode, specs, "0x%02X");
        _AST_FMT_NODE(AstParamDeclNode, ident);
        break;
    case AST_TRANS_UNIT:
        _AST_FMT_TYPE(AST_TRANS_UNIT);
        _AST_FMT_NODE_VEC(AstTransUnitNode, decls);
        break;
    default:
        _AST_FMT_TYPE("/* unknown */");
        break;
    }
    indent -= INDENT_SPACES;
    fprintf(stream, "%*s}\n", indent, "");
}

void ast_fmt(AstNode *node, FILE *stream) {
    _fmt(node, stream, 0);
}
