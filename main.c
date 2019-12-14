#include "ast.h"
#include "parser.h"

#include <stdlib.h>

int main() {
    AstNode *root = parse_stdin();
    if (root != NULL)
        ast_fmt(root, stdout);

    return !root;
}
