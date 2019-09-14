%{
#include "ast.h"
#include "parser.h"
#include "lexer.h"

#include <stdio.h>

void yyerror(yyscan_t*, AstNode**, char*);
%}

%output  "parser.c"
%defines "parser.h"

%define api.pure full
%define api.value.type { AstNode* }

%code requires {
typedef void *yyscan_t;
AstNode *parse_stdin();
}

%lex-param   { yyscan_t scanner }
%parse-param { yyscan_t scanner }
%parse-param { AstNode **root }


%token TOK_LPAREN "(" TOK_RPAREN ")"
%token TOK_OP_MUL "*" TOK_OP_DIV "/"
%token TOK_OP_ADD "+" TOK_OP_SUB "-"

%left "+" "-"
%left "*" "/"

%token TOK_SEMICOLON ";"
%token TOK_L_INT TOK_IDENT


%start statement

%%
expression
    : TOK_L_INT                     { }
    | TOK_IDENT                     { }
    | "(" expression ")"            { $$ = $2; }
    | expression "*" expression     { $$ = ast_binop('*', $1, $3); }
    | expression "/" expression     { $$ = ast_binop('/', $1, $3); }
    | expression "+" expression     { $$ = ast_binop('+', $1, $3); }
    | expression "-" expression     { $$ = ast_binop('-', $1, $3); }
    ;

statement
    : expression ";"                { *root = $1; }
    ;
%%

void yyerror(yyscan_t *loc, AstNode **node, char *err) {
    fprintf(stderr, "error: %s\n", err);
}

AstNode *parse_stdin() {
    yyscan_t scanner;
    AstNode *root;

    if (yylex_init(&scanner))
        return NULL;
    if (yyparse(scanner, &root))
        return NULL;

    yylex_destroy(scanner);

    return root;
}
