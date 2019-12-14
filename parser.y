%{
#include "ast.h"
#include "decl_spec.h"

#include "parser.h"
#include "lexer.h"

#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>

static bool _check_decl_specs(yyscan_t *scanner, ParserState *state,
                              DeclSpecs specs, DeclSpecs spec);

static void yyerror(yyscan_t *scanner, ParserState *state, const char *format,
                    ...);
%}

%output  "parser.c"
%defines "parser.h"

%define api.pure full
%define api.value.type union

%define parse.lac full
%define parse.error verbose

%code requires {
#define error(...) yyerror(scanner, state, __VA_ARGS__)

typedef struct ParserState {
    AstNode *ast_root;
} ParserState;

typedef void *yyscan_t;

AstNode *parse(FILE *stream);
}

%param { yyscan_t scanner }
%parse-param { ParserState *state }

%token <AstNode*> IDENT I_CONST F_CONST

%token IF ELSE WHILE DO FOR RETURN
%token INT SIGNED UNSIGNED VOID
%token INCR_OP DECR_OP LE_OP GE_OP EQ_OP NE_OP AND_OP OR_OP

%type <AstNode*> constant primary_expr postfix_expr unary_expr mult_expr
%type <AstNode*> add_expr rel_expr eq_expr and_expr xor_expr or_expr l_and_expr
%type <AstNode*> l_or_expr assignment_expr expr decl_stmt var_decl cmpd_stmt
%type <AstNode*> block_item expr_stmt selection_stmt iteration_stmt jump_stmt
%type <AstNode*> stmt fn_decl fn_param extern_decl trans_unit

%type <Vec*> fn_args var_decls block_items fn_params extern_decls
%type <DeclSpecs> decl_spec decl_specs

%expect 1

%start trans_unit

%%
_push_
    : %empty                            { /* TODO */ }
    ;

_pop_
    : %empty                            { /* TODO */ }
    ;

constant
    : I_CONST
    ;

primary_expr
    : constant
    | IDENT
    | '(' expr ')'                      { $$ = $2; }
    ;

postfix_expr
    : primary_expr
    | postfix_expr '(' ')'              { $$ = ast_fn_call($1, vec_new());  }
    | postfix_expr '(' fn_args ')'      { $$ = ast_fn_call($1, $3);         }
    | postfix_expr INCR_OP              { $$ = ast_unop(UNOP_POSTINCR, $1); }
    | postfix_expr DECR_OP              { $$ = ast_unop(UNOP_POSTDECR, $1); }

fn_args
    : assignment_expr                   { $$ = vec_from($1); }
    | fn_args ',' assignment_expr       { vec_pushb($1, $3); }
    ;

unary_expr
    : postfix_expr
    | '-' unary_expr                    { $$ = ast_unop(UNOP_NEG, $2);     }
    | '~' unary_expr                    { $$ = ast_unop(UNOP_NOT, $2);     }
    | '!' unary_expr                    { $$ = ast_unop(UNOP_L_NOT, $2);   }
    | INCR_OP unary_expr                { $$ = ast_unop(UNOP_PREINCR, $2); }
    | DECR_OP unary_expr                { $$ = ast_unop(UNOP_PREDECR, $2); }
    ;

mult_expr
    : unary_expr
    | mult_expr '*' unary_expr          { $$ = ast_binop(BINOP_MUL, $1, $3); }
    | mult_expr '/' unary_expr          { $$ = ast_binop(BINOP_DIV, $1, $3); }
    ;

add_expr
    : mult_expr
    | add_expr '+' unary_expr           { $$ = ast_binop(BINOP_ADD, $1, $3); }
    | add_expr '-' unary_expr           { $$ = ast_binop(BINOP_SUB, $1, $3); }
    ;

rel_expr
    : add_expr
    | rel_expr '<' unary_expr           { $$ = ast_binop(BINOP_LT, $1, $3); }
    | rel_expr '>' unary_expr           { $$ = ast_binop(BINOP_GT, $1, $3); }
    | rel_expr LE_OP unary_expr         { $$ = ast_binop(BINOP_LE, $1, $3); }
    | rel_expr GE_OP unary_expr         { $$ = ast_binop(BINOP_GE, $1, $3); }
    ;

eq_expr
    : rel_expr
    | eq_expr EQ_OP unary_expr          { $$ = ast_binop(BINOP_EQ, $1, $3); }
    | eq_expr NE_OP unary_expr          { $$ = ast_binop(BINOP_NE, $1, $3); }
    ;

and_expr
    : eq_expr
    | and_expr '&' unary_expr           { $$ = ast_binop(BINOP_AND, $1, $3); }
    ;

xor_expr
    : and_expr
    | xor_expr '^' unary_expr           { $$ = ast_binop(BINOP_XOR, $1, $3); }
    ;

or_expr
    : xor_expr
    | or_expr '|' unary_expr            { $$ = ast_binop(BINOP_OR, $1, $3); }
    ;

l_and_expr
    : or_expr
    | l_and_expr AND_OP unary_expr      { $$ = ast_binop(BINOP_L_AND, $1, $3); }
    ;

l_or_expr
    : l_and_expr
    | l_or_expr OR_OP unary_expr        { $$ = ast_binop(BINOP_L_OR, $1, $3); }
    ;

assignment_expr
    : l_or_expr
    | unary_expr '=' assignment_expr
    {
        $$ = ast_binop(BINOP_ASSIGN, $1, $3);
    }
    ; 

expr
    : assignment_expr
    | expr ',' assignment_expr          { $$ = ast_binop(BINOP_COMMA, $1, $3); }
    ;

decl_stmt
    : decl_specs ';'                    { $$ = ast_decl_stmt($1, vec_new()); }
    | decl_specs var_decls ';'          { $$ = ast_decl_stmt($1, $2);        }
    ;

decl_specs
    : decl_spec
    {
        $$ = $1;
    }
    | decl_specs decl_spec
    {
        if (_check_decl_specs(scanner, state, $1, $2))
            $$ = $1 | $2;
        else YYERROR;
    }
    ;

decl_spec
    : VOID                              { $$ = DECL_SPEC_VOID;     }
    | INT                               { $$ = DECL_SPEC_INT;      }
    | SIGNED                            { $$ = DECL_SPEC_SIGNED;   }
    | UNSIGNED                          { $$ = DECL_SPEC_UNSIGNED; }
    ;

var_decls
    : var_decl                          { $$ = vec_from($1); }
    | var_decls ',' var_decl            { vec_pushb($1, $3); }
    ;

var_decl
    : IDENT '=' assignment_expr         { $$ = ast_var_decl($1, $3);   }
    | IDENT                             { $$ = ast_var_decl($1, NULL); }
    ;

cmpd_stmt
    : '{' '}'                           { $$ = ast_cmpd_stmt(vec_new()); }
    | '{' _push_ block_items _pop_ '}'  { $$ = ast_cmpd_stmt($3);        }
    ;

block_items
    : block_item                        { $$ = vec_from($1); }
    | block_items block_item            { vec_pushb($1, $2); }
    ;

block_item
    : decl_stmt
    | stmt
    ;

expr_stmt
    : ';'                               { $$ = ast_expr_stmt(NULL); }
    | expr ';'                          { $$ = ast_expr_stmt($1);   }
    ;

selection_stmt
    : IF '(' expr ')' stmt              { $$ = ast_if_else_stmt($3, $5, NULL); }
    | IF '(' expr ')' stmt ELSE stmt    { $$ = ast_if_else_stmt($3, $5, $7);   }
    ;

iteration_stmt
    : WHILE '(' expr ')' stmt
    {
        $$ = ast_while_stmt($3, $5);
    }
    | DO stmt WHILE '(' expr ')' ';'
    {
        $$ = ast_do_while_stmt($2, $5);
    }
    | FOR _push_ '(' expr_stmt expr_stmt ')' stmt _pop_
    {
        $$ = ast_for_stmt($4, $5, ast_expr_stmt(NULL), $7);
    }
    | FOR _push_ '(' expr_stmt expr_stmt expr ')' stmt _pop_
    {
        $$ = ast_for_stmt($4, $5, $6, $8);
    }
    | FOR _push_ '(' decl_stmt expr_stmt ')' stmt _pop_
    {
        $$ = ast_for_stmt($4, $5, ast_expr_stmt(NULL), $7);
    }
    | FOR _push_ '(' decl_stmt expr_stmt expr ')' stmt _pop_
    {
        $$ = ast_for_stmt($4, $5, $6, $8);
    }
    ;

jump_stmt
    : RETURN ';'                        { $$ = ast_return_stmt(NULL); }
    | RETURN expr ';'                   { $$ = ast_return_stmt($2);   }
    ;

stmt
    : cmpd_stmt
    | expr_stmt
    | selection_stmt
    | iteration_stmt
    | jump_stmt
    ;

fn_decl
    : decl_specs IDENT _push_ '(' ')' _pop_ ';'
    {
        $$ = ast_fn_decl($2, vec_new(), NULL);
    }
    | decl_specs IDENT _push_ '(' fn_params ')' _pop_ ';'
    {
        $$ = ast_fn_decl($2, $5, NULL);
    }
    | decl_specs IDENT _push_ '(' ')' cmpd_stmt _pop_
    {
        $$ = ast_fn_decl($2, vec_new(), $6);
    }
    | decl_specs IDENT _push_ '(' fn_params ')' cmpd_stmt _pop_
    {
        $$ = ast_fn_decl($2, $5, $7);
    }
    ;

fn_params
    : fn_param                          { $$ = vec_from($1); }
    | fn_params ',' fn_param            { vec_pushb($1, $3); }
    ;

fn_param
    : decl_specs                        { $$ = ast_param_decl($1, NULL); }
    | decl_specs IDENT                  { $$ = ast_param_decl($1, $2);   }
    ;

trans_unit
    : extern_decls
    {
        $$ = state->ast_root = ast_trans_unit($1);
    }
    ;

extern_decls
    : extern_decl                       { $$ = vec_from($1); }
    | extern_decls extern_decl          { vec_pushb($1, $2); }
    ;

extern_decl
    : decl_stmt
    | fn_decl
    ;
%%

// Check whether adding `spec` to `specs` results in a valid combination of
// declaration specifiers
static bool _check_decl_specs(yyscan_t *scanner, ParserState *state,
                              DeclSpecs specs, DeclSpecs spec) {
    // Check for duplicate specifiers
    if (specs & spec)
        error("duplicate declaration specifier ‘%s’", decl_spec_str(spec));
    // Check for conflicting type specifiers
    else if (specs & DECL_SPEC_TYPE && spec & DECL_SPEC_TYPE)
        error("conflicting types ‘%s’ and ‘%s’ in declaration specifiers",
              decl_spec_str(specs & DECL_SPEC_TYPE), decl_spec_str(spec));
    // Check for conflicting sign specifiers
    else if (specs & DECL_SPEC_SIGN && spec & DECL_SPEC_SIGN)
        error("both ‘signed’ and ‘unsigned’ in declaration specifiers");
    // Check for a sign specifier and a type specifier other than int
    else if ((specs | spec) & (DECL_SPEC_TYPE & ~DECL_SPEC_INT) &&
             (specs | spec) & DECL_SPEC_SIGN)
        error("both ‘%s’ and ‘%s’ in declaration specifiers",
              decl_spec_str((specs | spec) & DECL_SPEC_TYPE & ~DECL_SPEC_INT),
              decl_spec_str((specs | spec) & DECL_SPEC_SIGN));
    else
        return true;
    return false;
}

static void yyerror(yyscan_t *scanner, ParserState *state, const char *format,
                    ...) {
    va_list args;

    fprintf(stderr, "\033[31;1merror:\033[0m ");

    va_start(args, format);
    vfprintf(stderr, format, args);
    va_end(args);

    fprintf(stderr, "\n");
}

AstNode *parse(FILE *stream) {
    ParserState state = { .ast_root = NULL };

    yyscan_t scanner;
    if (!yylex_init(&scanner)) {
        yyset_in(stream, scanner);
        yyparse(scanner, &state);
        yylex_destroy(scanner);
    }

    return state.ast_root;
}
