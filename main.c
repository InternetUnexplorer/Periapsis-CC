#include "ast.h"
#include "parser.h"

#include <stdio.h>
#include <stdlib.h>

int main() {
    AstNode *root = parse(stdin);
    if (root != NULL)
        ast_fmt(root, stdout);

    return !root;
}
