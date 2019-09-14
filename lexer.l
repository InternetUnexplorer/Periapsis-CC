%{
#include "ast.h"
#include "parser.h"
%}

%option outfile="lexer.c" header-file="lexer.h"

%option warn noinput nounput noyywrap nodefault
%option reentrant bison-bridge

%%
"("             return TOK_LPAREN;
")"             return TOK_RPAREN;
"*"             return TOK_OP_MUL;
"/"             return TOK_OP_DIV;
"+"             return TOK_OP_ADD;
"-"             return TOK_OP_SUB;

";"             return TOK_SEMICOLON;

[0-9]+          { *yylval = ast_l_int(atoi(yytext));
                  return TOK_L_INT; }
[a-z]+          { *yylval = ast_ident(strdup(yytext));
                  return TOK_IDENT; }

[ \t\n]         /* ignored */
.               /* ignored */
%%
