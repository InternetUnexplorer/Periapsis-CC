#ifndef AST_H
#define AST_H

#include "decl.h"
#include "ops.h"
#include "util/vec.h"

typedef enum AstNodeType {
    AST_IDENT,         // Identifier
    AST_I_CONST,       // Integer constant
    AST_F_CONST,       // Floating-point constant
    AST_FN_CALL,       // Function call
    AST_UNOP,          // Unary operation
    AST_BINOP,         // Binary operation
    AST_DECL_STMT,     // Declaration statement
    AST_VAR_DECL,      // Variable declaration
    AST_EXPR_STMT,     // Expression statement
    AST_CMPD_STMT,     // Compound statement
    AST_IF_ELSE_STMT,  // If-else statement
    AST_WHILE_STMT,    // While statement
    AST_DO_WHILE_STMT, // Do-while statement
    AST_FOR_STMT,      // For statement
    AST_RETURN_STMT,   // Return statement
    AST_FN_DECL,       // Function declaration
    AST_PARAM_DECL,    // Parameter declaration
    AST_TRANS_UNIT,    // Translation unit
} AstNodeType;

typedef struct AstNode {
    AstNodeType type;
} AstNode;

typedef struct AstIdentNode {
    AstNodeType type;
    char *ident;
} AstIdentNode;

typedef struct AstIConstNode {
    AstNodeType type;
    long value;
} AstIConstNode;

typedef struct AstFConstNode {
    AstNodeType type;
    double value;
} AstFConstNode;

typedef struct AstFnCallNode {
    AstNodeType type;
    AstNode *fn;
    Vec *args;
} AstFnCallNode;

typedef struct AstUnopNode {
    AstNodeType type;
    Unop op;
    AstNode *node;
} AstUnopNode;

typedef struct AstBinopNode {
    AstNodeType type;
    Binop op;
    AstNode *lhs, *rhs;
} AstBinopNode;

typedef struct AstDeclStmtNode {
    AstNodeType type;
    DeclSpecs specs;
    Vec *decls;
} AstDeclStmtNode;

typedef struct AstVarDeclNode {
    AstNodeType type;
    AstNode *ident, *value;
} AstVarDeclNode;

typedef struct AstExprStmtNode {
    AstNodeType type;
    AstNode *expr;
} AstExprStmtNode;

typedef struct AstCmpdStmtNode {
    AstNodeType type;
    Vec *stmts;
} AstCmpdStmtNode;

typedef struct AstIfElseStmtNode {
    AstNodeType type;
    AstNode *expr, *if_stmt, *else_stmt;
} AstIfElseStmtNode;

typedef struct AstWhileStmtNode {
    AstNodeType type;
    AstNode *expr, *stmt;
} AstWhileStmtNode;

typedef struct AstDoWhileStmtNode {
    AstNodeType type;
    AstNode *expr, *stmt;
} AstDoWhileStmtNode;

typedef struct AstForStmtNode {
    AstNodeType type;
    AstNode *init_expr, *cond_expr, *incr_expr, *stmt;
} AstForStmtNode;

typedef struct AstReturnStmtNode {
    AstNodeType type;
    AstNode *expr;
} AstReturnStmtNode;

typedef struct AstFnDeclNode {
    AstNodeType type;
    AstNode *ident;
    Vec *params;
    AstNode *body;
} AstFnDeclNode;

typedef struct AstParamDeclNode {
    AstNodeType type;
    DeclSpecs specs;
    AstNode *ident;
} AstParamDeclNode;

typedef struct AstTransUnitNode {
    AstNodeType type;
    Vec *decls;
} AstTransUnitNode;

AstNode *ast_ident(char *ident);
AstNode *ast_i_const(long iconst);
AstNode *ast_f_const(double fconst);
AstNode *ast_fn_call(AstNode *fn, Vec *args);
AstNode *ast_unop(Unop op, AstNode *node);
AstNode *ast_binop(Binop op, AstNode *lhs, AstNode *rhs);
AstNode *ast_decl_stmt(DeclSpecs specs, Vec *decls);
AstNode *ast_var_decl(AstNode *ident, AstNode *value);
AstNode *ast_expr_stmt(AstNode *expr);
AstNode *ast_cmpd_stmt(Vec *stmts);
AstNode *ast_if_else_stmt(AstNode *expr, AstNode *if_stmt, AstNode *else_stmt);
AstNode *ast_while_stmt(AstNode *expr, AstNode *stmt);
AstNode *ast_do_while_stmt(AstNode *expr, AstNode *stmt);
AstNode *ast_for_stmt(AstNode *init_expr, AstNode *cond_expr,
                      AstNode *incr_expr, AstNode *stmt);
AstNode *ast_return_stmt(AstNode *expr);
AstNode *ast_fn_decl(AstNode *ident, Vec *params, AstNode *body);
AstNode *ast_param_decl(DeclSpecs specs, AstNode *ident);
AstNode *ast_trans_unit(Vec *decls);

void ast_free(AstNode *node);
void ast_printf(AstNode *node);

#endif
