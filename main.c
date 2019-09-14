#include "ast.h"
#include "parser.h"
#include "lexer.h"

#include <stdlib.h>

int main(void) {
    AstNode *root = parse_stdin();
    if (root != NULL)
        ast_printf(root);

    return !root;
}
