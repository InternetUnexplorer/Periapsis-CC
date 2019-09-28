ALPHA [a-zA-Z_]
ALNUM [a-zA-Z_0-9]

HEXD [0-9a-fA-F]
DECD [0-9]
OCTD [0-7]

%{
#include "ast.h"
#include "parser.h"

static int _symbol(YYSTYPE *lval, char *text);
static int _i_const(YYSTYPE *lval, char *text);
static int _f_const(YYSTYPE *lval, char *text);
%}

%option outfile="lexer.c" header-file="lexer.h"

%option warn noinput nounput noyywrap nodefault debug
%option reentrant bison-bridge

%%
"do"                    return DO;
"else"                  return ELSE;
"float"                 return FLOAT;
"for"                   return FOR;
"if"                    return IF;
"int"                   return INT;
"return"                return RETURN;
"signed"                return SIGNED;
"unsigned"              return UNSIGNED;
"void"                  return VOID;
"while"                 return WHILE;

{ALPHA}{ALNUM}*         return _symbol(yylval, yytext);
[+-]?{DECD}+            return _i_const(yylval, yytext);
[+-]?"0"[xX]{HEXD}+     return _i_const(yylval, yytext);
[+-]?{DECD}+"."{DECD}+  return _f_const(yylval, yytext);

"++"                    return INCR_OP;
"--"                    return DECR_OP;
"&&"                    return AND_OP;
"||"                    return OR_OP;
"<="                    return LE_OP;
">="                    return GE_OP;
"=="                    return EQ_OP;
"!="                    return NE_OP;
";"                     return ';';
"{"                     return '{';
"}"                     return '}';
","                     return ',';
":"                     return ':';
"="                     return '=';
"("                     return '(';
")"                     return ')';
"["                     return '[';
"]"                     return ']';
"."                     return '.';
"&"                     return '&';
"!"                     return '!';
"~"                     return '~';
"-"                     return '-';
"+"                     return '+';
"*"                     return '*';
"/"                     return '/';
"%"                     return '%';
"<"                     return '<';
">"                     return '>';
"^"                     return '^';
"|"                     return '|';

[ \t\n]                 /* ignored */
.                       /* ignored */
%%

static int _symbol(YYSTYPE *lval, char *text) {
    lval->IDENT = ast_ident(strdup(text));
    return IDENT;
}

static int _i_const(YYSTYPE *lval, char *text) {
    // TODO: Check result
    lval->I_CONST = ast_i_const(strtoul(text, NULL, 0));
    return I_CONST;
}

static int _f_const(YYSTYPE *lval, char *text) {
    // TODO: Check result
    lval->F_CONST = ast_f_const(strtod(text, NULL));
    return F_CONST;
}
